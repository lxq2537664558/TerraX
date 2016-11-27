#include "NetManagerClient.h"

using namespace TerraX;

void NetManagerClient::Connect(const std::string& host, int port, bool bGate) {
	m_pConnector.reset(new Connector(PeerType_t::client, &m_loop, host/*"127.0.0.1"*/, port/*9991*/));
	if (bGate) {
		m_pConnector->SetNetEventCB(std::bind(&NetManagerClient::OnGateServer_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	}
	else {
		m_pConnector->SetNetEventCB(std::bind(&NetManagerClient::OnLoginServer_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	}
};

void NetManagerClient::SendPacket2Server(int32_t peer_info, google::protobuf::Message& packet) {
	//m_pConnector->SendPacket();
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