#pragma once
#include <memory>
#include "NetDefine.h"
#include "NetChannel.h"
namespace TerraX
{
	class Connector
	{
	public:
		Connector(PeerType_t peer_type, EventLoop* pLoop, const std::string& host, int port);
		~Connector() = default;
		void SendPacket(google::protobuf::Message& packet);
		bool IsConnected() { return m_pConnector->GetConnState() == ConnState_t::eConnected; }

		void SetNetEventCB(NetEvent_CB cb) { m_pConnector->RegNetEvent_Callback(cb); }
		
		//int32_t GetPeerInfo() { return m_pConnector->GetPeerInfo(); }
	private:
		NetChannelPtr m_pConnector;
	};
}