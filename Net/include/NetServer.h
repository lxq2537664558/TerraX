#pragma once

#include <event2/listener.h>
#include <mutex>
#include <string>
#include <queue>
#include <unordered_map>
#include "NetChannel.h"
#include "ComDef.h"
#include "NetDefine.h"

namespace TerraX
{
	class EventLoop;
	class NetChannel;
	class NetServer final
	{
		DISABLE_COPY(NetServer);
	public:
		NetServer(EventLoop* loop, int port, uint16_t max_conns);
		~NetServer();
	public:
		void SetThreadNum(int numThreads);

		void CloseAll();
		NetChannelPtr GetChannel(uint16_t nChannelIndex);
		NetChannelPtr GetChannel(PeerType_t peer_type, uint8_t peer_index);
		void RemoveChannel(NetChannelPtr& pChannel);

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
		std::queue<uint16_t> m_freeindexes;
		std::unordered_map<uint16_t, NetChannelPtr> m_mapChannels;
		const uint16_t m_maxconnections;
		NetEvent_CB m_NetEventCB;
		OnMessage_CB m_OnMessageCB;
	};

}