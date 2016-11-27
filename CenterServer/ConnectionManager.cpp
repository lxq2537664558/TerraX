#include "ConnectionManager.h"
#include "PacketDispatcher.h"
#include "CenterServer.h"
using namespace TerraX;

ConnectionManager::ConnectionManager(CenterServer& cs)
	: server(cs) {
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&ConnectionManager::OnMessage_Register, this, std::placeholders::_1, std::placeholders::_2)));

	for (uint8_t i = 1; i <= MAX_GATE_CONNECTION_COUNT; ++i) {
		m_queue_gateconnIds.push(i);
	}
	for (uint8_t i = 1; i <= MAX_GAME_CONNECTION_COUNT; ++i) {
		m_queue_gameconnIds.push(i);
	}
	for (uint8_t i = 1; i <= MAX_WORLD_CONNECTION_COUNT; ++i) {
		m_queue_worldconnIds.push(i);
	}
}

uint8_t ConnectionManager::GetAvailableConnIdx(PeerType_t peer_type)
{
	switch (peer_type)
	{
	case  PeerType_t::worldserver: 	{
		if (!m_queue_worldconnIds.empty()) {
			uint8_t conn_id = m_queue_worldconnIds.front();
			m_queue_worldconnIds.pop();
			return conn_id;
		}
		return 0;
	}
	case  PeerType_t::gameserver: {
		if (!m_queue_gameconnIds.empty()) {
			uint8_t conn_id = m_queue_gameconnIds.front();
			m_queue_gameconnIds.pop();
			return conn_id;
		}
		return 0;
	}
	case  PeerType_t::gateserver: {
		if (!m_queue_gateconnIds.empty()) {
			uint8_t conn_id = m_queue_gateconnIds.front();
			m_queue_gateconnIds.pop();
			return conn_id;
		}
		return 0;
	}
	default: break;
	}
	return 0;
}

void ConnectionManager::OnMessage_Register(NetChannelPtr& channel, PktRegisterServer& pkt) {
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(channel->GetChannelIndex() != 0 && pi.channel_index == 0);
	assert(pi.peer_index == 0);
	assert(pi.peer_type > (uint16_t)PeerType_t::client && pi.peer_type < (uint16_t)PeerType_t::peer_count);
	uint8_t conn_id = GetAvailableConnIdx(PeerType_t(pi.peer_type));
	if (conn_id == 0) {
		channel->ForceClose();
	}
	else {
		pi.peer_index = conn_id;
		pi.channel_index = channel->GetChannelIndex();
		channel->SetPeerInfo(pi.serialize());
		std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index) <<
			"\t ChannelIndex: " << pi.channel_index << std::endl;
		pkt.set_server_info(channel->GetPeerInfo());
		channel->SendMsg(0, pkt);
	}
}

void ConnectionManager::OnChannel_DisConnect(NetChannelPtr& channel)
{
	switch ((PeerType_t)channel->GetPeerType())
	{
	case  PeerType_t::worldserver: { 
		m_queue_worldconnIds.push(channel->GetPeerIndex());
		return;
	}
	case  PeerType_t::gameserver: {
		m_queue_gameconnIds.push(channel->GetPeerIndex());
		return;
	}
	case  PeerType_t::gateserver: {
		m_queue_gateconnIds.push(channel->GetPeerIndex());
		return;
	}
	default: break;
	}
}