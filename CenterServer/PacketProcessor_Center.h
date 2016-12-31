#pragma once

#include "PacketProcessor.h"
using namespace google::protobuf;

namespace TerraX
{
	class PacketProcessor_Center : public PacketProcessor
	{
		NOCOPY(PacketProcessor_Center);
		MAKEINSTANCE(PacketProcessor_Center);

	public:
		PacketProcessor_Center();
		~PacketProcessor_Center() = default;

	private:
		void ForwardPacket2FrontEnd(NetChannelPtr& pBackChannel, Packet* pkt) override final;
		void ForwardPacket2BackEnd(NetChannelPtr& pFrontChannel, Packet* pkt) override final;

		void DoFrontEnd_Connected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_Disconnected(NetChannelPtr& pChannel) override final;
		void DoFrontEnd_ConnBreak(NetChannelPtr& pChannel) override final;

	private:
		void OnMessage_Exit() {}
		void OnMessage_KickOut() {};

		void DoDisconnected() {}
	};
}