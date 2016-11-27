#include "Connector.h"

using namespace TerraX;

Connector::Connector(PeerType_t peer_type, EventLoop* pLoop, const std::string& host, int port)
	: m_pConnector(std::make_shared<NetChannel>(pLoop, host, port)) {
	m_pConnector->SetPeerInfo(uint8_t(peer_type) << 24);
}

void Connector::SendPacket(google::protobuf::Message& packet) {
	m_pConnector->SendMsg(0, packet);
}
