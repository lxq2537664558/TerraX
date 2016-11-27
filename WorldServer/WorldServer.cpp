#include "WorldServer.h"

using namespace TerraX;

WorldServer::WorldServer()
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&WorldServer::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2)));

}


bool WorldServer::Init()
{
	m_pConnector.reset(new Connector<WorldServer, PeerType_t::worldserver>(&m_loop, "127.0.0.1", 9995));
	m_pConnector->SetNetEventCB(std::bind(&WorldServer::OnConnector_NetEvent, this, std::placeholders::_1, std::placeholders::_2));

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

void WorldServer::OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt)
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(pi.peer_type == uint8_t(PeerType_t::worldserver));
	assert(pi.channel_index != 0 && channel->GetChannelIndex() == 0);
	std::cout << "Server: " << pi.server_name() <<
		"\t ChannelIndex: " << pi.channel_index << std::endl;
	channel->SetPeerType(pi.peer_type);
}

void WorldServer::OnConnector_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
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