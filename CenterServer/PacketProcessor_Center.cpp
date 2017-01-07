#include "ConnectionManager.h"
#include "PacketProcessor_Center.h"
#include "ServerManager_Center.h"
using namespace TerraX;

PacketProcessor_Center::PacketProcessor_Center() : PacketProcessor(PeerType_t::centerserver) {}

void PacketProcessor_Center::SendPacket(uint16_t channel_index, int dest_info, int owner_info,
                                        gpb::Message& msg)
{
    SendPacket2FrontEnd(channel_index, dest_info, owner_info, msg);
}

void PacketProcessor_Center::ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, Packet* pkt) {}

void PacketProcessor_Center::ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, Packet* pkt)
{
    PeerInfo pi(pkt->GetDesination());
    if (m_peer_type == pi.peer_type) {
        std::string packet_name = pkt->GetPacketName();
        pFrontChannel->OnMessage(pkt->GetOwnerInfo(), packet_name, pkt->GetPacketMsg(), pkt->GetMsgSize());
    } else {
        // send 2 gate
        uint16_t channel_index = (pi.peer_index == 0) ? pi.channel_index : pi.peer_index;
        auto pChannel = m_pFrontEnd->GetChannel(channel_index);
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
    ServerManager_Center::GetInstance().OnServerRemoved(pChannel->GetPeerInfo());

    m_pFrontEnd->RemoveChannel(pChannel);
}
void PacketProcessor_Center::DoFrontEnd_ConnBreak(NetChannelPtr& pChannel)
{
    // RemoveGuest();

    ServerManager_Center::GetInstance().OnServerRemoved(pChannel->GetPeerInfo());
    m_pFrontEnd->RemoveChannel(pChannel);
}