#include "tcp_server.h"

#include "eventloop.h"
#ifndef _WIN32
#include <unistd.h>
#include <arpa/inet.h>
#endif
#include "tcp_connection.h"
#include "socketopt-inl.h"

using namespace terra;

struct sockaddr* getListenSock(int port)
{
    static struct sockaddr_in sin;
    sin.sin_family = AF_INET;
    sin.sin_addr.s_addr = INADDR_ANY;
    sin.sin_port = htons(static_cast<uint16_t>(port));
    return (struct sockaddr*)&sin;
}

TcpServer::TcpServer(EventLoop* loop, int port, uint32_t max_conns, SocketEventCB sock_cb, MessageEventCB msg_cb)
	: evlistener_(evconnlistener_new_bind(loop->eventBase(), NewConnectionCallback, this,
		LEV_OPT_CLOSE_ON_FREE | LEV_OPT_REUSEABLE, -1, getListenSock(port),
		sizeof(struct sockaddr_in))),
	kMaxConns(max_conns),
	loop_(loop),
	socket_event_cb_(sock_cb),
	message_cb_(msg_cb)
{
}

TcpServer::~TcpServer() { evconnlistener_free(evlistener_); }

TcpConnection* TcpServer::GetTcpConnection(int fd)
{
	auto iter = tcp_conns_.find(fd);
	return (iter != tcp_conns_.end()) ? (iter->second).get() : nullptr;
}

void TcpServer::NewConnectionCallback(struct evconnlistener* listener, evutil_socket_t fd,
                                      struct sockaddr* address, int socklen, void* ctx)
{
    SetTcpNodelay(fd);
    printf("newConnectionCallback\n");
	struct sockaddr_in* addr_in = (sockaddr_in*) address;
	printf("ip: %s, port: %d\n", inet_ntoa(addr_in->sin_addr), addr_in->sin_port);
    TcpServer* self = static_cast<TcpServer*>(ctx);
    assert(self->evlistener_ == listener);
    self->OnConnect(fd);
}

void TcpServer::OnDisconnected(int fd) { tcp_conns_.erase(fd); }

void TcpServer::OnConnect(evutil_socket_t fd)
{
    assert(loop_);
    if (tcp_conns_.size() >= kMaxConns) {
        return;
    }
	std::unique_ptr<TcpConnection> conn(new TcpConnection(this, loop_->eventBase(), fd, socket_event_cb_, message_cb_));
    if (socket_event_cb_) {
        socket_event_cb_(conn.get(), ConnState_t::CONNECTED);
    }
    tcp_conns_[fd] = std::move(conn);
}