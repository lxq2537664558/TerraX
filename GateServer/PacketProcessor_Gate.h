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
		DISABLE_COPY(PacketProcessor_Gate);
		MAKE_INSTANCE(PacketProcessor_Gate);

	public:
		PacketProcessor_Gate();
		~PacketProcessor_Gate() = default;

		void SendPacket2Server(int dest_info, int owner_info, gpb::Message& msg);
		void SendPacket2Client(uint16_t channel_index, int owner_info, gpb::Message& msg);

	private:
		void Login2Center();
		void OnMessage_PktRegisterAck(S2SPacket::PktRegisterAck* pkt);

		void ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, PacketBase* pkt) override final;
		void ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, PacketBase* pkt) override final;

		void DoBackEnd_Connected(NetChannelPtr& pChannel) override final;
		void DoBackEnd_Disconnected(NetChannelPtr& pChannel) override final;
		void DoBackEnd_ConnBreak(NetChannelPtr& pChannel) override final;

		void DoFrontEnd_Connected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_Disconnected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_ConnBreak(NetChannelPtr& pChannel) override final;
	};
}