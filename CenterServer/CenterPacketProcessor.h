#pragma once

#include "PacketProcessor.h"
#include "proto/client_server.pb.h"
#include "proto/server_server.pb.h"
using namespace S2SPacket;
using namespace google::protobuf;

namespace TerraX
{
	class CenterPacketProcessor : public PacketProcessor
	{
		NOCOPY(CenterPacketProcessor);
		MAKEINSTANCE(CenterPacketProcessor);
	public:
		CenterPacketProcessor();
		~CenterPacketProcessor() = default;

		void ForwardPacket_FrontEnd(Packet* pkt) override final;
		void ForwardPacket_BackEnd(Packet* pkt) override final;
		void OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;
		void OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;
	};

}