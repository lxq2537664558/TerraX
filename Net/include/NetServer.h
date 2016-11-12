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
	private:
		static void NewConnectionCallback(struct evconnlistener* listener,
			evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
		static void DisconnectCallback(NetChannel*, void* ctx);
		static void* RunLoop(void* ptr);
	protected:
		void OnConnect(evutil_socket_t fd);
		virtual void OnDisconnect(NetChannel* pChannel);
	private:
		struct evconnlistener* m_evListener{ nullptr };
		std::vector<struct event_base*> m_loops;
		int m_currLoop{ 0 };
		std::mutex m_mutex;
		std::set<NetChannel*> m_channels;
	public:
		bool IsRegistered{ false };
		uint8_t m_ServerIndex{ 0 }; 
		PeerType_t m_PeerType{ PeerType_t::undefine };
	};

}