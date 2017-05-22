#include "server_accept_service.h"
#include "server_login_ack_service.h"
using namespace terra;

ServerAcceptService::ServerAcceptService(NetBaseModule& net)
	: net_(net)
{}
ServerAcceptService::~ServerAcceptService() {}

void ServerAcceptService::InitLoginAckService(PeerType_t peer, int max_conns)
{
	login_ack_.reset(new ServerLoginAckService(*this, peer, max_conns));
}

void ServerAcceptService::AcceptConnection(int port, uint32_t max_conns)
{
    server_.reset(new TcpServer(
        net_.get_event_loop(), port, max_conns,
        [this](TcpConnection* conn, ConnState_t conn_state) { this->OnSocketEvent(conn, conn_state); },
        [this](TcpConnection* conn, evbuffer* evbuf) { this->OnMessageEvent(conn, evbuf); }));
}

void ServerAcceptService::OnSocketEvent(TcpConnection* conn, ConnState_t conn_state)
{
    switch (conn_state) {
        case ConnState_t::CONNECTED:
            break;
        case ConnState_t::DISCONNECTED:
            // ReConnect();
			if (login_ack_)
			{
				login_ack_->OnLoginOut(conn);
			}
            break;
        default:
            break;
    }
}

void ServerAcceptService::OnMessageEvent(TcpConnection* conn, evbuffer* evbuf)
{
    net_.ProcessServerMessage(conn, evbuf);
}