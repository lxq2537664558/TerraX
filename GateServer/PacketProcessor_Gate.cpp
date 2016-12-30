#include "PacketProcessor_Gate.h"
#include "proto/client_server.pb.h"
#include "proto/server_server.pb.h"
#include "GuestManager.h"
#include "ServerManager.h"

using namespace S2SPacket;
using namespace TerraX;
PacketProcessor_Gate::PacketProcessor_Gate() : PacketProcessor(PeerType_t::gateserver)
{
    // PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterAck>(
    // new events_dynamic2(std::function<void(PktRegisterAck*)>(
    // std::bind(&GatePacketProcessor::OnMessage_LoginResult, this, std::placeholders::_1))));

    RegPacketHandler_Arg1(
        PktRegisterAck, std::bind(&PacketProcessor_Gate::OnMessage_LoginResult, this, std::placeholders::_1));
}

void PacketProcessor_Gate::ForwardPacket2FrontEnd(Packet* pkt)
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

void PacketProcessor_Gate::ForwardPacket2BackEnd(NetChannelPtr pFrontChannel, Packet* pkt) {
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
	//ServerManager::GetInstance().
	assert(m_pBackEnd);
	if (m_pBackEnd)
	{
		m_pBackEnd->SendMsg(pkt->GetBuffer(), pkt->Size());
	}
}

void PacketProcessor_Gate::OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) 
{
	switch (eEvent) {
	case NetEvent_t::eConnected:
		//CreateGuest();
		break;
	case NetEvent_t::eDisconnected:
		//RemoveGuest();
		break;
	default:
		break;
	}
}

void PacketProcessor_Gate::OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent)
{
    switch (eEvent) {
        case NetEvent_t::eConnected:
            Login2Center();
            break;
        case NetEvent_t::eConnectFailed:
			m_pBackEnd.reset();
            break;
		case NetEvent_t::eDisconnected:
			m_pBackEnd.reset();
            break;
        default:
            break;
    }
}

void PacketProcessor_Gate::Login2Center()
{
    PeerInfo pi_dest(PeerType_t::centerserver);
    PeerInfo pi_src(PeerType_t::gateserver);
    PktRegisterReq pkt;
    SendPacket2BackEnd(pi_dest.serialize(), pi_src.serialize(), pkt);
}

void PacketProcessor_Gate::OnMessage_LoginResult(PktRegisterAck* pkt)
{
    PeerInfo pi(pkt->server_info());
    assert(pi.peer_type == PeerType_t::gateserver);
    assert(pi.peer_index > 0);
    assert(pi.channel_index != 0 && m_pBackEnd->GetChannelIndex() == 0);
    std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index)
              << "\t ChannelIndex: " << pi.channel_index << std::endl;
    m_pBackEnd->SetPeerInfo(pkt->server_info());
}
