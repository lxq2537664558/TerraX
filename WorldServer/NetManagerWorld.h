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
	class NetManagerWorld
	{
		NOCOPY(NetManagerWorld);
		MAKEINSTANCE(NetManagerWorld);
	public:
		NetManagerWorld();
		~NetManagerWorld() = default;

		void Connect(const std::string& host, int port);

		void SendPacket(PeerType_t eDestPeer, google::protobuf::Message& packet);
		void SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet);

		void Tick() { m_loop.loop(); }

		void OnCenterServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);

		void OnMessageCenterServer(evbuffer* evbuf, NetChannelPtr& pChannel);
	private:
		void Register(int32_t peer_info);
		void OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt);
	private:
		EventLoop m_loop;
		NetChannelPtr m_pConnector;
		ProtobufCodecLite m_Codec;
	};

}