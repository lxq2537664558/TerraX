#pragma once

#include <event2/listener.h>
#include <vector>
#include <set>
#include <mutex>
#include <string>
#include "NetChannel.h"
#include "ComDef.h"

namespace TerraX
{
	class EventLoop;
	class NetChannel;
	class NetServer
	{
		NOCOPY(NetServer);
	public:
		NetServer(EventLoop* loop, int port);
	protected:
		~NetServer();
	public:
		void setThreadNum(int numThreads);

	private:
		static void newConnectionCallback(struct evconnlistener* listener,
			evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
		static void disconnectCallback(NetChannel*, void* ctx);
		static void* runLoop(void* ptr);

		void onConnect(evutil_socket_t fd);
		void onDisconnect(NetChannel*);
	private:
		struct evconnlistener* evListener_;
		std::vector<struct event_base*> loops_;
		int currLoop_;
		std::mutex mutex_;
		std::set<NetChannel*> channels_;
	};

}