#pragma once

#include <event2/listener.h>
#include <vector>
#include <set>
#include <mutex>
#include <string>
#include "RpcChannel.h"
#include "ComDef.h"

namespace TerraX
{
	class EventLoop;
	class RpcChannel;
	class RpcServer
	{
		NOCOPY(RpcServer);
	public:
		RpcServer(EventLoop* loop, int port);
	protected:
		~RpcServer();
	public:
		void setThreadNum(int numThreads);

	private:
		static void newConnectionCallback(struct evconnlistener* listener,
			evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
		static void disconnectCallback(RpcChannel*, void* ctx);
		static void* runLoop(void* ptr);

		void onConnect(evutil_socket_t fd);
		void onDisconnect(RpcChannel*);
	private:
		struct evconnlistener* evListener_;
		std::vector<struct event_base*> loops_;
		int currLoop_;
		std::mutex mutex_;
		std::set<RpcChannel*> channels_;
	};

}