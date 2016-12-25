#include "CenterPacketProcessor.h"
//#include "GuestManager.h"
#include "ConnectionManager.h"
using namespace TerraX;

CenterPacketProcessor::CenterPacketProcessor() : PacketProcessor(PeerType_t::centerserver) {
}

void CenterPacketProcessor::ForwardPacket_FrontEnd(Packet* pkt) {}

void CenterPacketProcessor::ForwardPacket_BackEnd(Packet* pkt) {}

void CenterPacketProcessor::OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) {}

void CenterPacketProcessor::OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent)
{
	switch (eEvent) {
	case TerraX::NetEvent_t::eConnected:
		break;
	case TerraX::NetEvent_t::eConnectFailed:
		break;
	case TerraX::NetEvent_t::eDisconnected:
		break;
	default:
		break;
	}
}