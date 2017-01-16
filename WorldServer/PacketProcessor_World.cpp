#include "PacketProcessor_World.h"
#include "ServerManager.h"
#include "proto/server_server.pb.h"

using namespace S2SPacket;
using namespace TerraX;
PacketProcessor_World::PacketProcessor_World() : PacketProcessor(PeerType_t::worldserver)
{
    REG_PACKET_HANDLER_ARG1(PktRegisterAck, this, OnMessage_PktRegisterAck);
}

void PacketProcessor_World::SendPacket(int dest_info, int owner_info, gpb::Message& msg)
{
    SendPacket2BackEnd(dest_info, owner_info, msg);
}

void PacketProcessor_World::ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, Packet* pkt)
{
    assert(pkt);
    int* pAllDest = nullptr;
    int nDestCount = 0;
    pkt->GetAllDesination(pAllDest, nDestCount);
    if (nDestCount <= 0 || !pAllDest) {
        return;
    }
    for (int i = 0; i < nDestCount; ++i) {
        int nDestInfo = pAllDest[i];
        PeerInfo pi(nDestInfo);
        if (pi.peer_type == m_peer_type) {
            std::string packet_name = pkt->GetPacketName();
            if (m_pBackEnd) {
                m_pBackEnd->OnMessage(pkt->GetOwnerInfo(), packet_name, pkt->GetPacketMsg(),
                                      pkt->GetMsgSize());
            }
        } else {
            assert(0);
        }
    }
}

void PacketProcessor_World::DoBackEnd_Connected(NetChannelPtr& pChannel) { Login2Center(); }

void PacketProcessor_World::DoBackEnd_Disconnected(NetChannelPtr& pChannel) { m_pBackEnd.reset(); }

void PacketProcessor_World::DoBackEnd_ConnBreak(NetChannelPtr& pChannel) { m_pBackEnd.reset(); }

void PacketProcessor_World::Login2Center()
{
    PeerInfo pi_dest(PeerType_t::centerserver);
    PeerInfo pi_src(PeerType_t::worldserver);
    PktRegisterReq pkt;
    SendPacket(pi_dest.serialize(), pi_src.serialize(), pkt);
}

void PacketProcessor_World::OnMessage_PktRegisterAck(PktRegisterAck* pkt)
{
    PeerInfo pi(pkt->server_info());
    std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index)
              << "\t ChannelIndex: " << pi.channel_index << std::endl;

    m_pBackEnd->SetPeerInfo(pkt->server_info());
    // m_pBackEnd.reset();
}
