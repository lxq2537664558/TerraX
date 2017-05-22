#include "server_conn_service.h"
#include "server_login_req_service.h"
#include "net_base_module.h"
#include "server_table.h"

using namespace terra;

ServerConnService::ServerConnService(NetBaseModule& net)
	: net_(net)
{

}
ServerConnService::~ServerConnService() {}

void ServerConnService::InitLoginReqService(PeerType_t peer)
{
	login_req_.reset(new ServerLoginReqService(*this, peer));
}

void ServerConnService::Connect2World()
{
	TcpConnection* conn = Connect(net_.get_conn_ip(), net_.get_conn_port(),
            [this](TcpConnection* conn, ConnState_t conn_state) { this->OnSocketEvent(conn, conn_state); },
            [this](TcpConnection* conn, evbuffer* evbuf) { this->OnMessageEvent(conn, evbuf); });

	ServerTable::GetInstance().AddServerInfo(PeerType_t::WORLDSERVER, WORD_SERVER_ID, net_.get_conn_ip(), net_.get_conn_port(), conn);
}

TcpConnection* ServerConnService::Connect(const char* ip, int port,
                                SocketEventCB sock_cb, MessageEventCB msg_cb)
{
	TcpConnection* conn = new TcpConnection(net_.get_event_loop(), ip, port, sock_cb, msg_cb);
	conns_[conn->get_fd()].reset(conn);
	return conn;
}

void ServerConnService::OnSocketEvent(TcpConnection* conn, ConnState_t conn_state)
{
    switch (conn_state) {
        case ConnState_t::CONNECTED:
			if (login_req_)
			{
				login_req_->Login2World(conn);
			}
            break;
        case ConnState_t::DISCONNECTED:
			if (login_req_)
			{
				login_req_->OnLoginOut(conn);
			}
			ServerTable::GetInstance().RemoveByConn(conn);
            conns_.erase(conn->get_fd());
            // ReConnect();
            break;
        default:
            break;
    }
}
void ServerConnService::OnMessageEvent(TcpConnection* conn, evbuffer* evbuf)
{
    net_.ProcessServerMessage(conn, evbuf);
}
