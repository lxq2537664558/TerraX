#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include "NetChannel.h"
#include "EventLoop.h"
#include "ProtobufCodecLite.h"
#include "NetServer.h"
#include "proto/server_server.pb.h"
#include "proto/client_server.pb.h"
using namespace S2SPacket;

namespace TerraX
{
	class NetManagerCenter
	{
		NOCOPY(NetManagerCenter);
		MAKEINSTANCE(NetManagerCenter);
	public:
		NetManagerCenter();
		~NetManagerCenter() = default;

		void Accept(int port, uint16_t max_connections);
		void SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet);
		void Tick() { m_loop.loop(); }

		void SendPacket(int32_t nDestPeerInfo, google::protobuf::Message& packet);

		void OnAcceptor_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);

		void OnMessageAcceptor(evbuffer* evbuf, NetChannelPtr& pChannel);
	private:
		void OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt);
		//void OnClientMessage
	private:
		EventLoop m_loop;
		std::unique_ptr<NetServer> m_pAcceptor;
		ProtobufCodecLite m_Codec;
	};

}