#include "NetManagerClient.h"

using namespace TerraX;

void NetManagerClient::Connect(const std::string& host, int port, bool bGate) {
	m_pConnector = std::make_shared<NetChannel>(m_loop, &m_loop, host/*"127.0.0.1"*/, port/*9991*/);
	m_pConnector->SetPeerInfo(uint8_t(PeerType_t::client) << 24);
	if (bGate) {
		m_pConnector->RegNetEvent_Callback(std::bind(&NetManagerClient::OnGateServer_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	}
	else {
		m_pConnector->RegNetEvent_Callback(std::bind(&NetManagerClient::OnLoginServer_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	}
};

void NetManagerClient::SendPacket(PeerType_t eDestPeer, google::protobuf::Message& packet) {
	PeerInfo pi(eDestPeer);
	m_Codec.SendMsg(m_pConnector, packet, 0, pi.serialize());
}

void NetManagerClient::OnGateServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if (eEvent == NetEvent_t::eConnected) {

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

void NetManagerClient::OnLoginServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if (eEvent == NetEvent_t::eConnected) {

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