#pragma once

#include "net_base_module.h"
#include "comm/net/tcp_connection.h"
#include <map>
#include <memory>

namespace terra
{
	class ServerLoginReqService;
	class ServerConnService
	{
	private:
		NetBaseModule& net_;
		std::map<int, std::unique_ptr<TcpConnection>> conns_;
		std::unique_ptr<ServerLoginReqService> login_req_;
	public:
		ServerConnService(NetBaseModule& net);
		~ServerConnService();

		const char* get_listen_ip() { return net_.get_listen_ip(); }
		int get_listen_port() { return net_.get_listen_port(); }

		void InitLoginReqService(PeerType_t peer);

		void Connect2World();
		TcpConnection* Connect(const char* ip, int port, SocketEventCB sock_cb, MessageEventCB msg_cb);
		
	private:

		void OnSocketEvent(TcpConnection* conn, ConnState_t conn_state);
		void OnMessageEvent(TcpConnection* conn, evbuffer* evbuf);
	};
}