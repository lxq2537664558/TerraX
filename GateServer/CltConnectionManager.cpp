#include "CltConnectionManager.h"
#include "PacketDispatcher.h"
#include "GateServer.h"

using namespace TerraX;

CltConnectionManager::CltConnectionManager(GateServer& cs)
	: server(cs) {
	for (uint16_t i = 1; i < Max_ClientConnection_Count; ++i) {
		m_freeindexes.push(i);
	}

	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterClient>(new PacketFunctor<PktRegisterClient>(
		std::bind(&CltConnectionManager::OnMessage_RegisterClient, this, std::placeholders::_1, std::placeholders::_2)));
}


void CltConnectionManager::OnMessage_RegisterClient(NetChannel& channel, PktRegisterClient& pkt) {
	int32_t client_info = pkt.client_info();
	PeerInfo pi;
	pi.parse(client_info);
	assert(pi.peer_type == (uint8_t)PeerType_t::client && pi.peer_index == 0 && pi.client_index == 0);
	auto pConnector = server.GetConnector();
	auto pAcceptor = server.GetAcceptor();
	assert(pAcceptor && pConnector);
	if (m_freeindexes.empty() || !pConnector->IsConnected()) {
		pAcceptor->SendPacket(channel, pkt);
		pAcceptor->ForceClose(channel);
	}
	else {
		pi.client_index = (uint16_t)m_freeindexes.front();
		m_freeindexes.pop();
		client_info = pi.serialize();
		pkt.set_client_info(client_info);
		pAcceptor->SendPacket(channel, pkt);
	}
}
