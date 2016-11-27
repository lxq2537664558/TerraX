#pragma once

#include <memory>
#include "NetDefine.h"
#include <functional>
#include "NetServer.h"
namespace TerraX
{
	class Acceptor
	{
	public:
		Acceptor(EventLoop* pLoop, int port, uint16_t max_connections);
		~Acceptor() = default;

		void SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet);
		//template<class Packet>
		//void SendPacket(int32_t peer_info, Packet& packet);

		void ForceClose(NetChannelPtr& channel);
		
		void SetNetEventCB(NetEvent_CB cb) { m_Acceptor.RegNetEvent_Callback(cb); }
	private:
		NetServer m_Acceptor;
	};
}