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
	class PacketProcessor_World : public PacketProcessor
	{
		NOCOPY(PacketProcessor_World);
		MAKEINSTANCE(PacketProcessor_World);

	public:
		PacketProcessor_World();
		~PacketProcessor_World() = default;

	private:
		void Login2Center();
		void OnMessage_PktRegisterAck(S2SPacket::PktRegisterAck* pkt);

		void DoBackEnd_Connected(NetChannelPtr& pChannel) override final;
		void DoBackEnd_Disconnected(NetChannelPtr& pChannel) override final;
		void DoBackEnd_ConnBreak(NetChannelPtr& pChannel) override final;

	};
}