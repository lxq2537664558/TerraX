
#include "tcp_connection.h"

#include <event2/buffer.h>
#include "eventloop.h"
#include "socketopt-inl.h"
#include "tcp_server.h"

using namespace terra;


TcpConnection::TcpConnection(EventLoop* loop, const std::string& host, int port, SocketEventCB sock_cb, MessageEventCB msg_cb) :
	loop_(loop),
	evconn_(bufferevent_socket_new(loop_->eventBase(), -1, BEV_OPT_CLOSE_ON_FREE)),
	socket_event_cb_(sock_cb),
	message_cb_(msg_cb)
{
	SetConnState(ConnState_t::CONNECTING);
	SetTcpNodelay(bufferevent_getfd(evconn_));
	bufferevent_setcb(evconn_, ReadCallback, WriteCallback, EventCallback, this);
	int result = bufferevent_socket_connect_hostname(evconn_, nullptr, AF_INET, host.c_str(), port);
	assert(result == 0);
	// int fd = bufferevent_getfd(evconn_);
}

TcpConnection::TcpConnection(TcpServer* server, struct event_base* base, int fd, SocketEventCB sock_cb, MessageEventCB msg_cb)
    : server_(server),
	evconn_(bufferevent_socket_new(base, fd, BEV_OPT_CLOSE_ON_FREE)),
	socket_event_cb_(sock_cb),
	message_cb_(msg_cb)
{
    SetConnState(ConnState_t::CONNECTED);
    bufferevent_setcb(evconn_, ReadCallback, WriteCallback, EventCallback, this);
    bufferevent_enable(evconn_, EV_READ | EV_WRITE);
}

TcpConnection::~TcpConnection()
{
    bufferevent_free(evconn_);
}

void TcpConnection::SendMsg(const char* buf, int len)
{
    if (conn_state_ == ConnState_t::CONNECTED) {
        bufferevent_write(evconn_, buf, len);
    }
}

void TcpConnection::OnRead()
{
    struct evbuffer* input = bufferevent_get_input(evconn_);
	if (message_cb_)
	{
		message_cb_(this, input);
	}
}

void TcpConnection::OnWrite()
{
    if (conn_state_ == ConnState_t::DISCONNECTING) {
        struct evbuffer* input = bufferevent_get_input(evconn_);
        std::size_t readable = evbuffer_get_length(input);
        // std::cout << "Writing: " << readable << std::endl;
        if (readable <= 0) {
            Disconnected();
        }
    }
}

void TcpConnection::ConnectError()
{
    connect_failed_ = true;
	Disconnected();
}

void TcpConnection::Connected()
{
    if (!connect_failed_) {
        SetConnState(ConnState_t::CONNECTED);
        bufferevent_enable(evconn_, EV_READ | EV_WRITE);

        if (socket_event_cb_) {
            socket_event_cb_(this, ConnState_t::CONNECTED);
        }
    }
}

void TcpConnection::Disconnected()
{
    SetConnState(ConnState_t::DISCONNECTED);
    if (socket_event_cb_) {
        socket_event_cb_(this, ConnState_t::DISCONNECTED);
	}
	if (server_)
	{
		server_->OnDisconnected(get_fd());
	}
}

void TcpConnection::ReadCallback(struct bufferevent* bev, void* ptr)
{
    TcpConnection* self = static_cast<TcpConnection*>(ptr);
    assert(self->evconn_ == bev);
    self->OnRead();
}

void TcpConnection::WriteCallback(struct bufferevent* bev, void* ptr)
{
    TcpConnection* self = static_cast<TcpConnection*>(ptr);
    assert(self->evconn_ == bev);
    self->OnWrite();
}

void TcpConnection::EventCallback(struct bufferevent* bev, short events, void* ptr)
{
    TcpConnection* self = static_cast<TcpConnection*>(ptr);
    if (events & BEV_EVENT_CONNECTED) {
        printf("connected\n");
        self->Connected();
    } else if (events & BEV_EVENT_EOF) {
        printf("disconnected\n");
        self->Disconnected();
    } else if (events & BEV_EVENT_ERROR) {
        printf("connect error\n");
        self->ConnectError();
    }
}