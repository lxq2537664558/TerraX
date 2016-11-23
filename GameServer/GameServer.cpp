#include "GameServer.h"

using namespace TerraX;

GameServer::GameServer()
{
	//you can use marco to wrapper it if you want;
	//PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		//std::bind(&GameServer::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2)));
}


bool GameServer::Init()
{
	m_pConnector.reset(new Connector<GameServer, PeerType_t::gameserver>(&m_loop, "127.0.0.1", 9995));

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
