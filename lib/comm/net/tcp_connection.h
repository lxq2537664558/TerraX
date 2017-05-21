#pragma once

#include <event2/bufferevent.h>
#include <map>
#include "base/macro.h"
#include "base/types.h"
#include "net_define.h"

namespace terra
{
	class EventLoop;

	class TcpConnection final
	{
		DISABLE_COPY(TcpConnection);
	private:
		class TcpServer* server_{ nullptr };
		EventLoop* loop_{ nullptr };
		struct bufferevent* evconn_{ nullptr };
		bool connect_failed_{ false };
		ConnState_t conn_state_{ ConnState_t::DISCONNECTED };
		SocketEventCB socket_event_cb_;
		MessageEventCB message_cb_;
	public:
		explicit TcpConnection(EventLoop* loop, const std::string& host, int port, SocketEventCB sock_cb, MessageEventCB msg_cb);
		explicit TcpConnection(class TcpServer* server, struct event_base* base, int fd, SocketEventCB sock_cb, MessageEventCB msg_cb);
		~TcpConnection();
		
		void SendMsg(const char* buf, int len);
		int get_fd() { return bufferevent_getfd(evconn_); }
	private:
		void OnRead();
		void OnWrite();

		void ConnectError();
		void Connected();
		void Disconnected();

		void SetConnState(ConnState_t eState) { conn_state_ = eState; }

		static void ReadCallback(struct bufferevent* bev, void* ptr);
		static void EventCallback(struct bufferevent* bev, short events, void* ptr);
		static void WriteCallback(struct bufferevent* bev, void* ptr);
	};
}