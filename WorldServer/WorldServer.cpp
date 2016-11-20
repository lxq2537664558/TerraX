#include "WorldServer.h"

using namespace TerraX;

WorldServer::WorldServer()
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&WorldServer::OnMessage_RegisterServerRet, this, std::placeholders::_1, std::placeholders::_2)));
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

void WorldServer::RegisterServer(PeerInfo& peerinfo)
{
	PktRegisterServer pkt;
	pkt.set_server_info(peerinfo.serialize());
	m_pConnector->SendPacket(pkt);
}

void WorldServer::OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt)
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	std::cout << "Server: " << int32_t(pi.peer_type) << "\tIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << int32_t(pi.client_index) << std::endl;
	if (pi.peer_index <= 0) {
		std::cout << "Register failed!" << std::endl;
		channel.ForceClose();
	}
}