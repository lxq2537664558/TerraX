#include "GuestManager.h"
#include "PacketProcessor_Gate.h"
#include "ServerManager.h"
#include "proto/client_server.pb.h"
#include "proto/server_server.pb.h"

using namespace S2SPacket;
using namespace TerraX;
PacketProcessor_Gate::PacketProcessor_Gate() : PacketProcessor(PeerType_t::gateserver)
{
    REG_PACKET_HANDLER_ARG1(PktRegisterAck, std::bind(&PacketProcessor_Gate::OnMessage_PktRegisterAck, this,
                                                      std::placeholders::_1));
}

void PacketProcessor_Gate::SendPacket2Server(int dest_info, int owner_info, gpb::Message& msg)
{
    SendPacket2BackEnd(dest_info, owner_info, msg);
}

void PacketProcessor_Gate::SendPacket2Client(uint16_t channel_index, int owner_info, gpb::Message& msg)
{
	NetChannelPtr pChannel = GetChannel_FrontEnd(channel_index);
	if (!pChannel) {
		return;
	}
	PacketC pkt(msg);
	pkt.SetOwner(owner_info);
	pChannel->SendMsg(pkt.buffer(), pkt.capacity());
}

void PacketProcessor_Gate::ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, PacketBase* pkt)
{
    assert(pkt);
    int* pAllDest = nullptr;
    int nDestCount = 0;
	PacketS* pktS = static_cast<PacketS*>(pkt);
    pktS->GetAllDesination(pAllDest, nDestCount);
    if (nDestCount <= 0 || !pAllDest) {
        return;
    }
    for (int i = 0; i < nDestCount; ++i) {
        int nDestInfo = pAllDest[i];
        PeerInfo pi(nDestInfo);
        if (pi.peer_type == m_peer_type) {
            std::string packet_name = pktS->GetPacketName();
            if (pBackChannel->GetPeerInfo() == nDestInfo) {
                if (m_pBackEnd) {
                    m_pBackEnd->OnMessage(pktS->GetOwnerInfo(), packet_name, pktS->GetPacketMsg(),
                                          pktS->GetMsgSize());
                }
            } else if (pi.peer_index == pBackChannel->GetChannelIndex()) {
                auto pChannel = m_pFrontEnd->GetChannel(pi.channel_index);
                if (pChannel) {
                    pChannel->OnMessage(pktS->GetOwnerInfo(), packet_name, pktS->GetPacketMsg(),
                                        pktS->GetMsgSize());
                }
            } else {
                // wrong data
            }
        }
        if (pi.peer_type == PeerType_t::client) {
            if (pi.peer_index == pBackChannel->GetChannelIndex()) {
                auto pChannel = m_pFrontEnd->GetChannel(pi.channel_index);
                if (pChannel) {
                    pChannel->SendMsg(pktS->GetDataBuffer(), pktS->GetDataSize());
                }
            }
        }
    }
}

void PacketProcessor_Gate::ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, PacketBase* pkt)
{
    assert(pkt);
	//pktC可在new创建时就在tail后面加上4个字节，这样就不用转了
	PacketC* pktC = static_cast<PacketC*>(pkt);
	//转变成pktS
    PeerInfo pi(pktC->GetCltDestionation());
    Guest* pGuest = GuestManager::GetInstance().GetGuest(pFrontChannel->GetPeerInfo());
    if (!pGuest) {
        assert(0);
        return;
    }
	int owner_info = (pGuest->GetAttachedAvatarID() == 0) ? pGuest->GetGuestID() : pGuest->GetAttachedAvatarID();
	pktC->SetOwner(owner_info);
    if (m_peer_type == pi.peer_type) {
        std::string packet_name = pktC->GetPacketName();
        pFrontChannel->OnMessage(owner_info, packet_name, pktC->GetPacketMsg(), pktC->GetMsgSize());
    } else {
		//转发时才需要创建pktS
		pktC->AppendDestination(pGuest->GetDestPeerInfo(pi.peer_type));
		if (m_pBackEnd) {
			m_pBackEnd->SendMsg(pktC->buffer(), pktC->capacity());
		}
    }
}

void PacketProcessor_Gate::DoBackEnd_Connected(NetChannelPtr& pChannel) { Login2Center(); }

void PacketProcessor_Gate::DoBackEnd_Disconnected(NetChannelPtr& pChannel)
{
    // m_pFrontEnd->CloseAll();
    m_pBackEnd.reset();
}

void PacketProcessor_Gate::DoBackEnd_ConnBreak(NetChannelPtr& pChannel)
{
    // m_pFrontEnd->CloseAll();
    m_pBackEnd.reset();
}

void PacketProcessor_Gate::DoFrontEnd_Connected(NetChannelPtr& pChannel)
{
    if (!m_pBackEnd) {
        // kick out client
        return;
    }
    pChannel->SetPeerType(PeerType_t::gateserver);
    pChannel->SetPeerIndex(static_cast<uint8_t>(m_pBackEnd->GetChannelIndex()));

    GuestManager::GetInstance().CreateGuest(pChannel->GetPeerInfo());
}
void PacketProcessor_Gate::DoFrontEnd_Disconnected(NetChannelPtr& pChannel)
{
    // RemoveGuest();

    m_pFrontEnd->RemoveChannel(pChannel);
}
void PacketProcessor_Gate::DoFrontEnd_ConnBreak(NetChannelPtr& pChannel)
{
    // RemoveGuest();
    m_pFrontEnd->RemoveChannel(pChannel);
}

void PacketProcessor_Gate::Login2Center()
{
    PeerInfo pi_dest(PeerType_t::centerserver);
    PeerInfo pi_src(PeerType_t::gateserver);
    PktRegisterReq pkt;
    SendPacket2Server(pi_dest.serialize(), pi_src.serialize(), pkt);
}

void PacketProcessor_Gate::OnMessage_PktRegisterAck(PktRegisterAck* pkt)
{
    PeerInfo pi(pkt->server_info());
    std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index)
              << "\t ChannelIndex: " << pi.channel_index << std::endl;

    m_pBackEnd->SetPeerInfo(pkt->server_info());
}
