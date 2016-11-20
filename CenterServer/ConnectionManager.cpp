#include "ConnectionManager.h"
#include "PacketDispatcher.h"
#include "CenterServer.h"
using namespace TerraX;

ConnectionManager::ConnectionManager(CenterServer& cs)
	: server(cs) {
	for (uint8_t i = 1; i < Max_WorldConnection_Count; ++i) {
		m_freeindexes[int(PeerType_t::worldserver)].push(i);
	}
	for (uint8_t i = 1; i < Max_GateConnection_Count; ++i) {
		m_freeindexes[int(PeerType_t::gateserver)].push(i);
	}
	for (uint8_t i = 1; i < Max_GameConnection_Count; ++i) {
		m_freeindexes[int(PeerType_t::gameserver)].push(i);
	}
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&ConnectionManager::OnMessage_Register, this, std::placeholders::_1, std::placeholders::_2)));
}

void ConnectionManager::OnMessage_Register(NetChannel& channel, PktRegisterServer& pkt) {
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(pi.peer_index == 0 && pi.client_index == 0);
	auto pAcceptor = server.GetAcceptor();
	assert(pAcceptor);
	if (m_freeindexes[pi.peer_type].empty()) {
		pAcceptor->SendPacket(channel, pkt);
		pAcceptor->ForceClose(channel);
	}
	else {
		pi.peer_index = (uint8_t)m_freeindexes[pi.peer_type].front();
		m_freeindexes[pi.peer_type].pop();
		server_info = pi.serialize();
		pkt.set_server_info(server_info);
		pAcceptor->SendPacket(channel, pkt);
		m_mapRegisterChannels[server_info] = &channel;
	}
}

void ConnectionManager::UnRegister(NetChannel* pChannel) {
	for (auto it = m_mapRegisterChannels.begin(); 
		it != m_mapRegisterChannels.end(); ++it) {
		if (it->second == pChannel) {
			PeerInfo pi;
			pi.parse(it->first);
			m_freeindexes[pi.peer_type].push(pi.peer_index);
			m_mapRegisterChannels.erase(it);
			return;
		}
	}
	assert(1);//impossible!
}