#pragma once

#include <event2/listener.h>
#include <vector>
#include <set>
#include <mutex>
#include <string>
#include <array>
#include <queue>
#include "NetChannel.h"
#include "ComDef.h"
#include "NetDefine.h"

namespace TerraX
{
	class EventLoop;
	class NetChannel;
	class NetServer final
	{
		NOCOPY(NetServer);
	public:
		using DisconnectEvent_CB = std::function<void(NetChannel& channel)>;
		NetServer(EventLoop* loop, int port, uint16_t max_conns);
		~NetServer();
	public:
		void SetThreadNum(int numThreads);

		void ForceClose(NetChannel& channel);
		void ForceCloseAll();

		void RegDisconnected_Callback(DisconnectEvent_CB cb) {
			m_DisconnectedCB = cb;
		}
	private:
		static void NewConnectionCallback(struct evconnlistener* listener,
			evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
		static void DisconnectCallback(NetChannel*, void* ctx);
		static void* RunLoop(void* ptr);

		void OnConnect(evutil_socket_t fd);
		void OnDisconnect(NetChannel* pChannel);
	private:
		struct evconnlistener* m_evListener{ nullptr };
		std::vector<struct event_base*> m_loops;
		int m_currLoop{ 0 };
		std::mutex m_mutex;
	private:
		std::vector<NetChannel*> m_vecChannels;
		std::queue<uint16_t> m_freeindexes;
		const uint16_t m_maxconnections;
		DisconnectEvent_CB m_DisconnectedCB;
	};

}