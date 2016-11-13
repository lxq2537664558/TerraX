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
	using PacketFnCB = std::function<void(class packet*)>;
	enum class ConnState_t 
	{ 
		eDisconnected, 
		eConnecting, 
		eConnected, 
		eDisconnecting 
	};
	class NetChannel
	{
		NOCOPY(NetChannel);
	public:
		using disconnect_cb = std::function<void(NetChannel*, void* ptr)> ;

		explicit NetChannel(struct event_base *base, int fd);
		explicit NetChannel(EventLoop* loop, const std::string& host, int port);
		~NetChannel();
		void SetDisconnectCb(disconnect_cb cb, void* ptr);
		
		void SendMessage(int flag, google::protobuf::Message& msg);
		bool OnMessage(const std::string& strMsgType, const char* pBuffer, const int nBufferSize);

		ConnState_t GetConnState() { return m_eState; }
		void ForceClose();

	protected:
		void OnRead();
		void OnWrite();

		virtual void ConnectFailed();
		virtual void Connected();
		virtual void Disconnected();

		void SetConnState(ConnState_t eState) { m_eState = eState; }

		static void ReadCallback(struct bufferevent *bev, void *ptr);
		static void EventCallback(struct bufferevent *bev, short events, void *ptr);
		static void WriteCallback(struct bufferevent *bev, void *ptr);
	private:
		struct bufferevent* m_evConn{ nullptr };
		bool m_connectFailed{ false };
		disconnect_cb m_disconnect_cb{ nullptr };
		void* m_ptr{ nullptr };
	private:
		uint16_t m_nChannelIndex{ 0 }; // channel index
		ConnState_t m_eState{ ConnState_t::eDisconnected };
	};


}