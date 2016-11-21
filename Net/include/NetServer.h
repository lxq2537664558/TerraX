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
	class NetServer final
	{
		NOCOPY(NetServer);
	public:
		using DisconnectEvent_CB = std::function<void(int32_t peerinfo)>;
		NetServer(EventLoop* loop, int port);
		~NetServer();
	public:
		void SetThreadNum(int numThreads);

		void ForceClose(NetChannel& pChannel);
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
		std::set<NetChannel*> m_channels;
	private:
		DisconnectEvent_CB m_DisconnectedCB;
	};

}