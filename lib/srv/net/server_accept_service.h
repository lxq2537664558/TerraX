#pragma once

#include "net_base_module.h"
#include "comm/net/tcp_server.h"
#include "comm/proto/server_server.pb.h"

namespace terra
{
	class ServerLoginAckService;
	class ServerAcceptService
	{
	private:
		NetBaseModule& net_;
		std::unique_ptr<TcpServer> server_;
		std::unique_ptr<ServerLoginAckService> login_ack_;
	public:
		ServerAcceptService(NetBaseModule& net);
		~ServerAcceptService();
		
		void InitLoginAckService(std::unique_ptr<ServerLoginAckService>& login_ack);

		void AcceptConnection(int port, uint32_t max_conns);

		void OnSocketEvent(TcpConnection* conn, ConnState_t conn_state);
		void OnMessageEvent(TcpConnection* conn, evbuffer* evbuf);

	};
}