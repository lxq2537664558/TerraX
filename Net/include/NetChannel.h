#pragma once
#include <iostream>
#include <map>
#include <atomic>
#include <cstdint>
#include <mutex>
#include <event2/bufferevent.h>
#include "PacketDispatcher.h"
#include "ComDef.h"
#include "NetDefine.h"

namespace TerraX
{
	class EventLoop;
	//typedef std::shared_ptr<google::protobuf::Message> MessagePtr;
	class NetChannel final : public std::enable_shared_from_this<NetChannel>
	{
		NOCOPY(NetChannel);
	public:
		using SrvDisconnect_CB = std::function<void(NetChannelPtr&)> ;

		explicit NetChannel(struct event_base *base, int fd);
		explicit NetChannel(EventLoop* loop, const std::string& host, int port);
		~NetChannel();
		void SetDisconnectCb(SrvDisconnect_CB cb) { m_SrvDisconnectCB = cb; };
		
		void SendMsg(int flag, google::protobuf::Message& msg);
		bool OnMessage(const std::string& strMsgType, const char* pBuffer, const int nBufferSize);

		void SetPeerType(uint16_t peer_type) { m_peer_info = (peer_type << 16) + (m_peer_info & 0x0000FFFF); }
		uint16_t GetPeerType() const { return (m_peer_info & 0xFFFF0000) >> 16; }
		void SetChannelIndex(uint16_t idx) { m_peer_info = (m_peer_info & 0xFFFF0000) + idx; }
		uint16_t GetChannelIndex() const { return m_peer_info & 0x0000FFFF; }
		int32_t GetPeerInfo() const { return m_peer_info; }

		ConnState_t GetConnState() const { return m_eState; }
		void ForceClose();

		void RegNetEvent_Callback(NetEvent_CB cb) { m_NetEventCB = cb; }

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
		SrvDisconnect_CB m_SrvDisconnectCB;
	private:
		NetEvent_CB m_NetEventCB;
		ConnState_t m_eState{ ConnState_t::eDisconnected };
		int32_t m_peer_info{ 0 };
	};


}