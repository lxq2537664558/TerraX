#include "CenterServer.h"
#include "ConnectionManager.h"
#include "PacketProcessor_Center.h"
#include "ServerManager_Center.h"
#include <algorithm>
using namespace TerraX;

PacketProcessor_Center::PacketProcessor_Center() : PacketProcessor(PeerType_t::centerserver) {}

void PacketProcessor_Center::SendPacket(uint16_t channel_index, int dest_info, int owner_info,
                                        gpb::Message& msg)
{
    SendPacket2FrontEnd(channel_index, dest_info, owner_info, msg);
}

void PacketProcessor_Center::ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, PacketBase* pkt) {}

void PacketProcessor_Center::ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, PacketBase* pkt)
{
    int* pAllDest = nullptr;
    int nDestCount = 0;
	PacketS* pktS = static_cast<PacketS*>(pkt);
	pktS->GetAllDesination(pAllDest, nDestCount);
    if (nDestCount <= 0 || !pAllDest) {
        return;
    }
    uint16_t arrChannels[MAX_CONNECTION];
    int nChannelCount = 0;
    for (int i = 0; i < nDestCount; ++i) {
        PeerInfo pi(pAllDest[i]);
        if (m_peer_type == pi.peer_type) {
            std::string packet_name = pktS->GetPacketName();
            pFrontChannel->OnMessage(pktS->GetOwnerInfo(), packet_name, pktS->GetPacketMsg(),
				pktS->GetMsgSize());
        } else {
            // send 2 gate
            uint16_t channel_index = (pi.peer_index == 0) ? pi.channel_index : pi.peer_index;
            if (std::find(arrChannels, arrChannels + nChannelCount, channel_index) ==
                (arrChannels + nChannelCount)) {
                arrChannels[nChannelCount++] = channel_index;
            }
        }
        for (int i = 0; i < nChannelCount; ++i) {
            auto pChannel = m_pFrontEnd->GetChannel(arrChannels[i]);
            if (pChannel) {
                pChannel->SendMsg(pktS->buffer(), pktS->GetPacketSize());
            }
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