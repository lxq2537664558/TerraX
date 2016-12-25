#pragma once

#include "PacketProcessor.h"
#include "proto/client_server.pb.h"
#include "proto/server_server.pb.h"
using namespace S2SPacket;
using namespace google::protobuf;

namespace TerraX
{
	class Packet;
	class GatePacketProcessor : public PacketProcessor
	{
		NOCOPY(GatePacketProcessor);
		MAKEINSTANCE(GatePacketProcessor);

	public:
		GatePacketProcessor();
		~GatePacketProcessor() = default;

	private:
		void Login2Center();
		void OnMessage_LoginResult(PktRegisterAck* pkt);

		void ForwardPacket_FrontEnd(Packet* pkt) override final;
		void ForwardPacket_BackEnd(Packet* pkt) override final;
		void OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;
		void OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;
	};
}