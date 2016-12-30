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

		void ForwardPacket2FrontEnd(Packet* pkt) override final;
		void ForwardPacket2BackEnd(NetChannelPtr pFrontChannel, Packet* pkt) override final;
		void OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;
		void OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) override final;


	private:
		void OnMessage_Exit(){}
		void OnMessage_KickOut() {};

		void DoDisconnected(){}
	};

}