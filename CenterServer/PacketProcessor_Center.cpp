#include "ConnectionManager.h"
#include "PacketProcessor_Center.h"
#include "ServerManager_Center.h"
using namespace TerraX;

PacketProcessor_Center::PacketProcessor_Center() : PacketProcessor(PeerType_t::centerserver) {}

void PacketProcessor_Center::SendPacket(int channel_info, int dest_info, int owner_info, gpb::Message& msg)
{
    SendPacket2FrontEnd(channel_info, dest_info, owner_info, msg);
}

void PacketProcessor_Center::ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, Packet* pkt) {}

void PacketProcessor_Center::ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, Packet* pkt)
{
    PeerInfo pi(pkt->GetDesination());
    if (m_peer_type == pi.peer_type) {
        std::string packet_name = pkt->GetPacketName();
        pFrontChannel->OnMessage(pFrontChannel->GetPeerInfo(), pkt->GetOwnerInfo(), packet_name,
                                 pkt->GetPacketMsg(), pkt->GetMsgSize());
    } else {
        auto pChannel = m_pFrontEnd->GetChannel(pi.channel_index);
        if (pChannel) {
            pChannel->SendMsg(pkt->GetBuffer(), pkt->Size());
        }
    }
}

void PacketProcessor_Center::DoFrontEnd_Connected(NetChannelPtr& pChannel)
{
    // CreateGuest();
}
void PacketProcessor_Center::DoFrontEnd_Disconnected(NetChannelPtr& pChannel)
{
    // RemoveGuest();
    ConnectionManager::GetInstance().OnChannel_DisConnect(pChannel);
    ServerManager_Center::GetInstance().OnServerRemoved(pChannel->GetPeerInfo());

    m_pFrontEnd->RemoveChannel(pChannel);
}
void PacketProcessor_Center::DoFrontEnd_ConnBreak(NetChannelPtr& pChannel)
{
    // RemoveGuest();
    ConnectionManager::GetInstance().OnChannel_DisConnect(pChannel);

    ServerManager_Center::GetInstance().OnServerRemoved(pChannel->GetPeerInfo());
    m_pFrontEnd->RemoveChannel(pChannel);
}