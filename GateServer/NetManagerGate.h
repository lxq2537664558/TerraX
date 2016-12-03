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
	class NetManagerGate
	{
		NOCOPY(NetManagerGate);
		MAKEINSTANCE(NetManagerGate);
	public:
		NetManagerGate();
		~NetManagerGate() = default;

		void Connect(const std::string& host, int port);
		void Accept(EventLoop* pLoop, int port, uint16_t max_connections);
		void SendPacket(PeerType_t eDestPeer, google::protobuf::Message& packet);
		void Tick() { m_loop.loop(); }

		void OnClient_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);
		void OnCenterServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);

		void OnMessageCenterServer(evbuffer* evbuf, NetChannelPtr& pChannel);
		void OnMessageClient(evbuffer* evbuf, NetChannelPtr& pChannel);
	private:
		void Register(int32_t peer_info);
		void OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt);
		//void OnClientMessage
	private:
		EventLoop m_loop;
		NetChannelPtr m_pConnector;
		std::unique_ptr<NetServer> m_pAcceptor;
		//CODEC m_codec;
		ProtobufCodecLite m_Codec;
	};

}