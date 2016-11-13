#pragma once

#include <event2/listener.h>
#include <vector>
#include <set>
#include <mutex>
#include <string>
#include "NetChannel.h"
#include "ComDef.h"
#include "NetDefine.h"

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
		void SetThreadNum(int numThreads);

		void ForceClose(NetChannel& pChannel);
		void ForceCloseAll();
	private:
		static void NewConnectionCallback(struct evconnlistener* listener,
			evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
		static void DisconnectCallback(NetChannel*, void* ctx);
		static void* RunLoop(void* ptr);
	protected:
		virtual void OnConnect(evutil_socket_t fd);
		virtual void OnDisconnect(NetChannel* pChannel);
	private:
		struct evconnlistener* m_evListener{ nullptr };
		std::vector<struct event_base*> m_loops;
		int m_currLoop{ 0 };
		std::mutex m_mutex;
		std::set<NetChannel*> m_channels;
	};

}