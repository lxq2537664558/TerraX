#pragma once

#include "base/types.h"
#include "comm/net/eventloop.h"
#include "comm/net/tcp_connection.h"
#include "comm/net/tcp_server.h"
#include "base/module_interface.h"

namespace terra
{
	class TcpConnection;
	class NetBaseModule
	{
	private:
		EventLoop loop_;
		std::string listen_ip_;
		int listen_port_{ 0 };
		std::string conn_ip_;
		int conn_port_{ 0 };
	public:
		NetBaseModule() = default;
		virtual ~NetBaseModule(){}

		EventLoop* get_event_loop() { return &loop_; }
		const char* get_conn_ip() { return conn_ip_.c_str(); }
		int get_conn_port() { return conn_port_; }
		const char* get_listen_ip() { return listen_ip_.c_str(); }
		int get_listen_port() { return listen_port_;  }

		virtual void ProcessServerMessage(TcpConnection* conn, evbuffer* evbuf);
	protected:
		void InitConnectInfo(const std::string& ip, int port);
		void InitListenInfo(const std::string& ip, int port);
	};

}