#include "NetManagerGate.h"

using namespace TerraX;

NetManagerGate::NetManagerGate() {
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&NetManagerGate::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2)));
}


void NetManagerGate::Connect(const std::string& host, int port) {
	m_pConnector = std::make_shared<NetChannel>(m_loop, &m_loop, host, port);
	m_pConnector->SetPeerInfo(uint8_t(PeerType_t::gateserver) << 24);
	m_pConnector->RegNetEvent_Callback(std::bind(&NetManagerGate::OnCenterServer_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	m_pConnector->RegOnMessage_Callback(std::bind(&NetManagerGate::OnMessageCenterServer, this, std::placeholders::_1, std::placeholders::_2));
};

void NetManagerGate::Accept(EventLoop* pLoop, int port, uint16_t max_connections) {
	m_pAcceptor.reset(new NetServer(pLoop, port, max_connections));
	m_pAcceptor->RegNetEvent_Callback(std::bind(&NetManagerGate::OnClient_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	m_pConnector->RegOnMessage_Callback(std::bind(&NetManagerGate::OnMessageClient, this, std::placeholders::_1, std::placeholders::_2));
}

void NetManagerGate::OnMessageCenterServer(evbuffer* evbuf, NetChannelPtr& pChannel)
{

}

void NetManagerGate::OnMessageClient(evbuffer* evbuf, NetChannelPtr& pChannel)
{
	int32_t nDestPeerInfo; 
	bool res = m_Codec.ExtractMsgDestInfo(evbuf, pChannel, nDestPeerInfo);
	PeerInfo pi;
	pi.parse(nDestPeerInfo);
	if (pi.peer_type != PeerType_t::gateserver)
	{
		//×ª·¢
		//packetforwarding 
		m_pConnector->SendMsg(evbuf);
	}
	else
	{
		ErrorCode_t eErrCode = m_Codec.ReadMsg(evbuf, pChannel);
	}
}

void NetManagerGate::SendPacket(PeerType_t eDestPeer, google::protobuf::Message& packet) {
	PeerInfo pi(eDestPeer);
	m_Codec.SendMsg(m_pConnector, packet, 0, pi.serialize());
}

void NetManagerGate::OnClient_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
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

void NetManagerGate::OnCenterServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
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

void NetManagerGate::Register(int32_t peer_info)
{
	PktRegisterServer pkt;
	pkt.set_server_info(peer_info);
	m_pConnector->SendPacket(pkt);
}

void NetManagerGate::OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt)
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(pi.peer_type == PeerType_t::gateserver);
	assert(pi.peer_index > 0);
	assert(pi.channel_index != 0 && channel->GetChannelIndex() == 0);
	std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << pi.channel_index << std::endl;
	channel->SetPeerInfo(server_info);
}