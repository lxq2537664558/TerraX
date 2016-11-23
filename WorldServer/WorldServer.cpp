#include "WorldServer.h"

using namespace TerraX;

WorldServer::WorldServer()
{
}


bool WorldServer::Init()
{
	m_pConnector.reset(new Connector<WorldServer, PeerType_t::worldserver>(&m_loop, "127.0.0.1", 9995));

	return true;
}

void WorldServer::Run()
{
	while (!m_bExit)
	{
		m_loop.loop();
	}
}

void WorldServer::Register(int32_t peer_info)
{
	PktRegisterServer pkt;
	pkt.set_server_info(peer_info);
	m_pConnector->SendPacket(pkt);
}
