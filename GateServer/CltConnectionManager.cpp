#include "CltConnectionManager.h"
#include "PacketDispatcher.h"
#include "GateServer.h"

using namespace TerraX;

CltConnectionManager::CltConnectionManager(GateServer& cs)
	: server(cs) {
	for (uint16_t i = 1; i < Max_ClientConnection_Count; ++i) {
		m_freeindexes.push(i);
	}

	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&CltConnectionManager::OnMessage_RegisterClient, this, std::placeholders::_1, std::placeholders::_2)));
}


void CltConnectionManager::OnMessage_RegisterClient(NetChannel& channel, PktRegisterServer& pkt) {
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(pi.peer_index != 0 && pi.client_index == 0);
	auto pConnector = server.GetConnector();
	auto pAcceptor = server.GetAcceptor();
	assert(pAcceptor && pConnector);
	if (m_freeindexes.empty() || pConnector->IsConnected()) {
		pAcceptor->SendPacket(channel, pkt);
		pAcceptor->ForceClose(channel);
	}
	else {
		pi.client_index = (uint16_t)m_freeindexes.front();
		m_freeindexes.pop();
		server_info = pi.serialize();
		pkt.set_server_info(server_info);
		pAcceptor->SendPacket(channel, pkt);
	}
}
