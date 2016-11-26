#pragma once
#include <memory>
#include "NetDefine.h"
#include "NetChannel.h"
namespace TerraX
{
	template<class T, PeerType_t peertype>
	class Connector
	{
	public:
		Connector(EventLoop* pLoop, const std::string& host, int port);
		~Connector() = default;
		template<class Packet> 
		void SendPacket(Packet& packet); 
		bool IsConnected() { return m_pConnector->GetConnState() == ConnState_t::eConnected; }

		void SetNetEventCB(NetEvent_CB cb) { m_pConnector->RegNetEvent_Callback(cb); }
		
		//void OnMessageReceived();
	private:
		NetChannelPtr m_pConnector;
	};

	template<class T, PeerType_t peertype>
	Connector<T, peertype>::Connector(EventLoop* pLoop, const std::string& host, int port)
		: m_pConnector(std::make_shared<NetChannel>(pLoop, host, port)) {
		m_pConnector->SetPeerType(uint16_t(peertype));
		m_pConnector->SetChannelIndex(0);
	}

	template<class T, PeerType_t peertype>
	template<class Packet>
	void Connector<T, peertype>::SendPacket(Packet& packet) {
		m_pConnector->SendMsg(0, packet);
	}

}