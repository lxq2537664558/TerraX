#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include "NetChannel.h"
#include "EventLoop.h"
#include "ProtobufCodecLite.h"

namespace TerraX
{
	class NetManagerClient
	{
		NOCOPY(NetManagerClient);
		MAKEINSTANCE(NetManagerClient);
	public:
		NetManagerClient() = default;
		~NetManagerClient() = default;

		void Connect(const std::string& host, int port, bool bGate);
		void SendPacket(PeerType_t eDestPeer, google::protobuf::Message& packet);
		void Tick() { m_loop.loop();}

		void OnGateServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);
		void OnLoginServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);
	private:
		EventLoop m_loop;
		NetChannelPtr m_pConnector;
		//CODEC m_codec;
		ProtobufCodecLite m_Codec;
	};

}