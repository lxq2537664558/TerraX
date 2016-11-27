#include "GameServer.h"

using namespace TerraX;

GameServer::GameServer()
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&GameServer::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2)));
}


bool GameServer::Init()
{
	m_pConnector.reset(new Connector(PeerType_t::gameserver, &m_loop, "127.0.0.1", 9995));
	m_pConnector->SetNetEventCB(std::bind(&GameServer::OnConnector_NetEvent, this, std::placeholders::_1, std::placeholders::_2));

	return true;
}

void GameServer::Run()
{
	while (!m_bExit)
	{
		m_loop.loop();
	}
}

void GameServer::Register(int32_t peer_info)
{
	PktRegisterServer pkt;
	pkt.set_server_info(peer_info);
	m_pConnector->SendPacket(pkt);
}

void GameServer::OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt)
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(pi.peer_type == uint8_t(PeerType_t::gameserver));
	assert(pi.peer_index > 0);
	assert(pi.channel_index != 0 && channel->GetChannelIndex() == 0);
	std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << pi.channel_index << std::endl;
	channel->SetPeerInfo(server_info);
}

void GameServer::OnConnector_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if (eEvent == NetEvent_t::eConnected) {
		Register(channel->GetPeerInfo());
	}
	else if (eEvent == NetEvent_t::eConnectFailed) {
		// do exit...
	}
	else if (eEvent == NetEvent_t::eDisconnected) {
		// do disconnect...
	}
	else {
		// unknown event
	}
}