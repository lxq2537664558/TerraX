#include "GameServer.h"

using namespace TerraX;

GameServer::GameServer()
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&GameServer::OnMessage_RegisterServerRet, this, std::placeholders::_1, std::placeholders::_2)));
}

bool GameServer::Init()
{
	m_pNetChannel.reset(new NetChannel(&m_loop, "127.0.0.1", 9995));
	m_pNetChannel->RegConnected_Callback(std::bind(&GameServer::Connected, this));
	m_pNetChannel->RegConnectFailed_Callback(std::bind(&GameServer::ConnectFailed, this));
	m_pNetChannel->RegDisconnected_Callback(std::bind(&GameServer::Disconnected, this));

	return true;
}

void GameServer::Run()
{
	while (!m_bExit)
	{
		m_loop.loop();
	}
}


void GameServer::ConnectFailed()
{
	//Reconnect
}

void GameServer::Connected()
{
	RegisterServer();
}

void GameServer::Disconnected()
{
}

void GameServer::RegisterServer()
{
	PktRegisterServer pkt;
	pkt.set_server_info(GetServerInfo());
	SendPacket(pkt);
}

void GameServer::OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt) 
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	std::cout << "Server: " << int32_t(pi.peer_type) << "\tIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << int32_t(pi.channel_index) << std::endl;
	if (pi.peer_index <= 0) {
		std::cout << "Register failed!" << std::endl;
		channel.ForceClose();
	}
}