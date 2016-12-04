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
		NetServer(EventLoop* loop, int port, uint16_t max_conns);
		~NetServer();
	public:
		void SetThreadNum(int numThreads);

		void ForceClose(NetChannelPtr& channel);
		void ForceCloseAll();

		NetChannelPtr GetChannel(uint16_t nChannelIndex);
		NetChannelPtr GetChannel(PeerType_t peer_type, uint8_t peer_index);

		void RegNetEvent_Callback(NetEvent_CB cb) { m_NetEventCB = cb; }
		void RegOnMessage_Callback(OnMessage_CB cb) { m_OnMessageCB = cb; }
	private:
		static void NewConnectionCallback(struct evconnlistener* listener,
			evutil_socket_t fd, struct sockaddr* address, int socklen, void* ctx);
		static void* RunLoop(void* ptr);

		void OnConnect(evutil_socket_t fd);
		void OnDisconnect(NetChannelPtr& pChannel);
		void DisconnectCallback(NetChannelPtr&);
	private:
		struct evconnlistener* m_evListener{ nullptr };
		std::vector<struct event_base*> m_loops;
		int m_currLoop{ 0 };
		std::mutex m_mutex;
	private:
		std::vector<NetChannelPtr> m_vecChannels;
		std::queue<uint16_t> m_freeindexes;
		const uint16_t m_maxconnections;
		NetEvent_CB m_NetEventCB;
		OnMessage_CB m_OnMessageCB;
	};

}