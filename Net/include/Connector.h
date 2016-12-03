#pragma once
#include <memory>
#include "NetDefine.h"
#include "NetChannel.h"
#include "ProtobufCodecLite.h"
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
		void SetOnMessageCB(OnMessage_CB cb) { m_pConnector->RegOnMessage_Callback(cb); }

		//int32_t GetPeerInfo() { return m_pConnector->GetPeerInfo(); }
	private:
		NetChannelPtr m_pConnector;
		//CODEC m_codec;
	};

	Connector::Connector(PeerType_t peer_type, EventLoop* pLoop, const std::string& host, int port)
		: m_pConnector(std::make_shared<NetChannel>(pLoop, host, port)) {
		m_pConnector->SetPeerInfo(uint8_t(peer_type) << 24);
	}

	void Connector::SendPacket(google::protobuf::Message& packet) {
		//m_codec.SendMsg(m_pConnector, packet);
		//ProtobufCodecLiteT<T>::GetInstance().SendMsg(m_pConnector, packet);
		//m_pConnector->SendMsg(0, packet);
		//此处发送已经打包好的数据即可
	}
	//在server层处理数据，需要考虑到back-end , front-end问题
}