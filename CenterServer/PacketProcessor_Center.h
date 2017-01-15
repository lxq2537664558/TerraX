#pragma once

#include "PacketProcessor.h"
using namespace google::protobuf;

namespace TerraX
{
	class PacketProcessor_Center : public PacketProcessor
	{
		DISABLE_COPY(PacketProcessor_Center);
		MAKE_INSTANCE(PacketProcessor_Center);

	public:
		PacketProcessor_Center();
		~PacketProcessor_Center() = default;

		void SendPacket(uint16_t channel_index, int dest_info, int owner_info, gpb::Message& msg);

	private:
		void ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, PacketBase* pkt) override final;
		void ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, PacketBase* pkt) override final;

		void DoFrontEnd_Connected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_Disconnected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_ConnBreak(NetChannelPtr& pChannel) override final;

	private:
		void OnMessage_Exit() {}
		void OnMessage_KickOut() {};

		void DoDisconnected() {}
	};
}