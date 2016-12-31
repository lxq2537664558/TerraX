#include "PacketProcessor_Gate.h"
#include "proto/client_server.pb.h"
#include "proto/server_server.pb.h"
#include "GuestManager.h"
#include "ServerManager.h"

using namespace S2SPacket;
using namespace TerraX;
PacketProcessor_Gate::PacketProcessor_Gate() : PacketProcessor(PeerType_t::gateserver)
{
    RegPacketHandler_Arg1(
        PktRegisterAck, std::bind(&PacketProcessor_Gate::OnMessage_PktRegisterAck, this, std::placeholders::_1));
}

void PacketProcessor_Gate::ForwardPacket2FrontEnd(NetChannelPtr& pBackChannel, Packet* pkt)
{
	assert(pkt);
	PeerInfo pi(pkt->GetDesination());
	assert(pi.peer_type == PeerType_t::client);
	auto pChannel = m_pFrontEnd->GetChannel(pi.channel_index);
	if (pChannel)
	{
		pChannel->SendMsg(pkt->GetBuffer(), pkt->Size());
	}
}

void PacketProcessor_Gate::ForwardPacket2BackEnd(NetChannelPtr& pFrontChannel, Packet* pkt) {
	assert(pkt);
	PeerInfo pi(pkt->GetDesination());
	Guest* pGuest = GuestManager::GetInstance().GetGuest(pFrontChannel->GetPeerInfo());
	if (pGuest->GetGuestGameState() < GuestState_t::eEnteringScene)
	{
		pkt->SetOwner(pGuest->GetGuestID());
	}
	else
	{
		pkt->SetOwner(pGuest->GetAttachAvatarID());
	}
	pkt->SetDestination(pGuest->GetDestPeerInfo(pi.peer_type));
	//ServerManager::GetInstance().
	assert(m_pBackEnd);
	if (m_pBackEnd)
	{
		m_pBackEnd->SendMsg(pkt->GetBuffer(), pkt->Size());
	}
}

void PacketProcessor_Gate::DoBackEnd_Connected(NetChannelPtr& pChannel)
{
	Login2Center();
}

void PacketProcessor_Gate::DoBackEnd_Disconnected(NetChannelPtr& pChannel)
{
	m_pBackEnd.reset();
}

void PacketProcessor_Gate::DoBackEnd_ConnBreak(NetChannelPtr& pChannel)
{
	m_pBackEnd.reset();
}

void PacketProcessor_Gate::DoFrontEnd_Connected(NetChannelPtr& pChannel)
{
	if (!m_pBackEnd)
	{
		//kick out client
		return;
	}
	pChannel->SetPeerType(PeerType_t::client);
	pChannel->SetPeerIndex(m_pBackEnd->GetPeerIndex());
	GuestManager::GetInstance().CreateGuest(pChannel->GetPeerInfo());
	//CreateGuest();
}
void PacketProcessor_Gate::DoFrontEnd_Disconnected(NetChannelPtr& pChannel)
{
	//RemoveGuest();

	m_pFrontEnd->RemoveChannel(pChannel);
}
void PacketProcessor_Gate::DoFrontEnd_ConnBreak(NetChannelPtr& pChannel)
{
	//RemoveGuest();
	m_pFrontEnd->RemoveChannel(pChannel);
}

void PacketProcessor_Gate::Login2Center()
{
    PeerInfo pi_dest(PeerType_t::centerserver);
    PeerInfo pi_src(PeerType_t::gateserver);
    PktRegisterReq pkt;
    SendPacket2BackEnd(pi_dest.serialize(), pi_src.serialize(), pkt);
}

void PacketProcessor_Gate::OnMessage_PktRegisterAck(PktRegisterAck* pkt)
{
    PeerInfo pi(pkt->server_info());
    assert(pi.peer_type == PeerType_t::gateserver);
    assert(pi.peer_index > 0);
    assert(pi.channel_index != 0 && m_pBackEnd->GetChannelIndex() == 0);
    std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index)
              << "\t ChannelIndex: " << pi.channel_index << std::endl;
    m_pBackEnd->SetPeerInfo(pkt->server_info());

	//m_pBackEnd.reset();
}
