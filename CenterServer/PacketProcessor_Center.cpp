#include "ConnectionManager.h"
#include "PacketProcessor_Center.h"
using namespace TerraX;

PacketProcessor_Center::PacketProcessor_Center() : PacketProcessor(PeerType_t::centerserver) {}


void PacketProcessor_Center::SendPacket(int channel_info, int dest_info, int owner_info, gpb::Message& msg)
{
	SendPacket2FrontEnd(channel_info, dest_info, owner_info, msg);
}

void PacketProcessor_Center::ForwardPacket2FrontEnd(NetChannelPtr& pBackChannel, Packet* pkt) {}

void PacketProcessor_Center::ForwardPacket2BackEnd(NetChannelPtr& pFrontChannel, Packet* pkt) {}

void PacketProcessor_Center::DoFrontEnd_Connected(NetChannelPtr& pChannel)
{
    // CreateGuest();
}
void PacketProcessor_Center::DoFrontEnd_Disconnected(NetChannelPtr& pChannel)
{
    // RemoveGuest();
	ConnectionManager::GetInstance().OnChannel_DisConnect(pChannel);
	ServerManager::GetInstance().OnServerRemoved(pChannel->GetPeerInfo());

    m_pFrontEnd->RemoveChannel(pChannel);
}
void PacketProcessor_Center::DoFrontEnd_ConnBreak(NetChannelPtr& pChannel)
{
    // RemoveGuest();
	ConnectionManager::GetInstance().OnChannel_DisConnect(pChannel);

	ServerManager::GetInstance().OnServerRemoved(pChannel->GetPeerInfo());
    m_pFrontEnd->RemoveChannel(pChannel);
}