#include "PacketProcessor_World.h"
#include "proto/server_server.pb.h"
#include "ServerManager.h"

using namespace S2SPacket;
using namespace TerraX;
PacketProcessor_World::PacketProcessor_World() : PacketProcessor(PeerType_t::worldserver)
{
	// PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterAck>(
	// new events_dynamic2(std::function<void(PktRegisterAck*)>(
	// std::bind(&GatePacketProcessor::OnMessage_LoginResult, this, std::placeholders::_1))));

	RegPacketHandler_Arg1(
		PktRegisterAck, std::bind(&PacketProcessor_World::OnMessage_PktRegisterAck, this, std::placeholders::_1));
}



void PacketProcessor_World::DoBackEnd_Connected(NetChannelPtr& pChannel)
{
	Login2Center();
}

void PacketProcessor_World::DoBackEnd_Disconnected(NetChannelPtr& pChannel)
{
	m_pBackEnd.reset();
}

void PacketProcessor_World::DoBackEnd_ConnBreak(NetChannelPtr& pChannel)
{
	m_pBackEnd.reset();
}

void PacketProcessor_World::Login2Center()
{
	PeerInfo pi_dest(PeerType_t::centerserver);
	PeerInfo pi_src(PeerType_t::worldserver);
	PktRegisterReq pkt;
	SendPacket2BackEnd(pi_dest.serialize(), pi_src.serialize(), pkt);
}

void PacketProcessor_World::OnMessage_PktRegisterAck(PktRegisterAck* pkt)
{
	PeerInfo pi(pkt->server_info());
	assert(pi.peer_type == PeerType_t::worldserver);
	assert(pi.peer_index > 0);
	assert(pi.channel_index != 0 && m_pBackEnd->GetChannelIndex() == 0);
	std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index)
		<< "\t ChannelIndex: " << pi.channel_index << std::endl;
	m_pBackEnd->SetPeerInfo(pkt->server_info());

	//m_pBackEnd.reset();
}
