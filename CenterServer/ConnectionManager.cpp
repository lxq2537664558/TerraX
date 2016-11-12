#include "ConnectionManager.h"
#include "PacketDispatcher.h"
#include "CenterServer.h"
using namespace TerraX;

ConnectionManager::ConnectionManager(CenterServer& cs)
	: server(cs) {
	for (int i = 0; i < Max_WorldConnection_Count; ++i) {
		m_freeindexes[int(PeerType_t::worldserver)].push(i);
	}
	for (int i = 0; i < Max_GateConnection_Count; ++i) {
		m_freeindexes[int(PeerType_t::gateserver)].push(i);
	}
	for (int i = 0; i < Max_GameConnection_Count; ++i) {
		m_freeindexes[int(PeerType_t::gameserver)].push(i);
	}
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&ConnectionManager::OnMessage_RegisterServer, this, std::placeholders::_1, std::placeholders::_2)));
}

void ConnectionManager::OnMessage_RegisterServer(NetChannel& channel, PktRegisterServer& pkt) {
	int32_t server_info = pkt.server_info();
	ServerInfo_t si;
	si.parse(server_info);
	assert(si.peer_type > (uint8_t)PeerType_t::undefine && si.peer_type < (uint8_t)PeerType_t::peer_count);
	assert(si.peer_index == 0 && si.channel_index == 0);
	if (m_freeindexes[si.peer_type].empty()) {
		server.SendPacket(channel, pkt);
		//server.forceClose
	}
	else {
		si.peer_index = (uint8_t)m_freeindexes[si.peer_type].front();
		m_freeindexes[si.peer_type].pop();
		server_info = si.serialize();
		pkt.set_server_info(server_info);
		server.SendPacket(channel, pkt);
		m_mapRegisterChannels[server_info] = &channel;
	}
}

void ConnectionManager::UnregisterServer(NetChannel* pChannel) {
	for (auto it = m_mapRegisterChannels.begin(); 
		it != m_mapRegisterChannels.end(); ++it) {
		if (it->second == pChannel) {
			m_mapRegisterChannels.erase(it);
			return;
		}
	}
	assert(1);//impossible!
}