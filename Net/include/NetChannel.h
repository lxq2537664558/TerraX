#pragma once
#include <iostream>
#include <functional>
#include <map>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <event2/bufferevent.h>
#include "PacketDispatcher.h"
#include "ComDef.h"

namespace TerraX
{
	class EventLoop;
	enum class ConnState_t 
	{ 
		eDisconnected, 
		eConnecting, 
		eConnected, 
		eDisconnecting 
	};
	class NetChannel final
	{
		NOCOPY(NetChannel);
	public:
		using disconnect_cb = std::function<void(NetChannel*, void* ptr)> ;
		using ConnectionEvent_CB = std::function<void()>;

		explicit NetChannel(struct event_base *base, int fd);
		explicit NetChannel(EventLoop* loop, const std::string& host, int port);
		~NetChannel();
		void SetDisconnectCb(disconnect_cb cb, void* ptr);
		
		void SendMsg(int flag, google::protobuf::Message& msg);
		bool OnMessage(const std::string& strMsgType, const char* pBuffer, const int nBufferSize);

		ConnState_t GetConnState() { return m_eState; }
		void ForceClose();

		void RegConnectFailed_Callback(ConnectionEvent_CB cb) { m_ConnectFailedCB = cb; }
		void RegConnected_Callback(ConnectionEvent_CB cb) { m_ConnectedCB = cb; }
		void RegDisconnected_Callback(ConnectionEvent_CB cb) { m_DisconnectedCB = cb; }

	private:
		void OnRead();
		void OnWrite();

		void ConnectFailed();
		void Connected();
		void Disconnected();

		void SetConnState(ConnState_t eState) { m_eState = eState; }

		static void ReadCallback(struct bufferevent *bev, void *ptr);
		static void EventCallback(struct bufferevent *bev, short events, void *ptr);
		static void WriteCallback(struct bufferevent *bev, void *ptr);
	private:
		struct bufferevent* m_evConn{ nullptr };
		bool m_connectFailed{ false };
		disconnect_cb m_disconnect_cb;
		void* m_ptr{ nullptr };
	private:
		ConnectionEvent_CB m_ConnectFailedCB;
		ConnectionEvent_CB m_ConnectedCB;
		ConnectionEvent_CB m_DisconnectedCB;
		ConnState_t m_eState{ ConnState_t::eDisconnected };
	};


}