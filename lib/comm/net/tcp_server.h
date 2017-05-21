#pragma once

#include <event2/listener.h>
#include <map>
#include "net_define.h"

namespace terra
{
	class EventLoop;

	class TcpServer
	{
	private:
		const uint32_t kMaxConns;
		EventLoop* loop_{ nullptr };
		struct evconnlistener* evlistener_{ nullptr };
		SocketEventCB socket_event_cb_;
		MessageEventCB message_cb_;
		std::map<int, std::unique_ptr<TcpConnection>> tcp_conns_;
	public:
		TcpServer(EventLoop* loop, int port, uint32_t max_conns, SocketEventCB sock_cb, MessageEventCB msg_cb);
		~TcpServer();

		//void CloseAll();
		//void Close(int fd);
		TcpConnection* GetTcpConnection(int fd);

		void OnDisconnected(int fd);
	private:
		void OnConnect(evutil_socket_t fd);
		static void NewConnectionCallback(struct evconnlistener* listener,
			evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
	};
}