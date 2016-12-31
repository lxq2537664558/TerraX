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
		void OnMessage_PktRegisterAck(S2SPacket::PktRegisterAck* pkt);

		void ForwardPacket2FrontEnd(NetChannelPtr& pBackChannel, Packet* pkt) override final;
		void ForwardPacket2BackEnd(NetChannelPtr& pFrontChannel, Packet* pkt) override final;

		void DoBackEnd_Connected(NetChannelPtr& pChannel) override final;
		void DoBackEnd_Disconnected(NetChannelPtr& pChannel) override final;
		void DoBackEnd_ConnBreak(NetChannelPtr& pChannel) override final;

		void DoFrontEnd_Connected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_Disconnected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_ConnBreak(NetChannelPtr& pChannel) override final;
	};
}