#include "PacketProcessor_Center.h"
//#include "GuestManager.h"
#include "ConnectionManager.h"
using namespace TerraX;

PacketProcessor_Center::PacketProcessor_Center() : PacketProcessor(PeerType_t::centerserver) {
}

void PacketProcessor_Center::ForwardPacket2FrontEnd(Packet* pkt) {}

void PacketProcessor_Center::ForwardPacket2BackEnd(NetChannelPtr pFrontChannel, Packet* pkt) {}

void PacketProcessor_Center::OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) {}

void PacketProcessor_Center::OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent)
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