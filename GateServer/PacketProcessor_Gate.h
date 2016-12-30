#pragma once

#include "PacketProcessor.h"
using namespace google::protobuf;
namespace S2SPacket
{
	class PktRegisterAck;
}

namespace TerraX
{
	class Packet;
	class PacketProcessor_Gate : public PacketProcessor
	{
		NOCOPY(PacketProcessor_Gate);
		MAKEINSTANCE(PacketProcessor_Gate);

	public:
		PacketProcessor_Gate();
		~PacketProcessor_Gate() = default;

	private:
		void Login2Center();
		void OnMessage_LoginResult(S2SPacket::PktRegisterAck* pkt);

		void ForwardPacket2FrontEnd(Packet* pkt) override final;
		void ForwardPacket2BackEnd(NetChannelPtr pFrontChannel, Packet* pkt) override final;
		void OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;
		void OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;
	};
}