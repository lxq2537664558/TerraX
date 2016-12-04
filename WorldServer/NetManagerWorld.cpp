#include "NetManagerWorld.h"
using namespace TerraX;

NetManagerWorld::NetManagerWorld() {
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&NetManagerWorld::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2)));
}

void NetManagerWorld::Connect(const std::string& host, int port) {
	m_pConnector = std::make_shared<NetChannel>(&m_loop, host, port);
	m_pConnector->SetPeerInfo(uint8_t(PeerType_t::worldserver) << 24);
	m_pConnector->RegNetEvent_Callback(std::bind(&NetManagerWorld::OnCenterServer_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	m_pConnector->RegOnMessage_Callback(std::bind(&NetManagerWorld::OnMessageCenterServer, this, std::placeholders::_1, std::placeholders::_2));
}

void NetManagerWorld::OnMessageCenterServer(evbuffer* evbuf, NetChannelPtr& pChannel)
{
	struct evbuffer* evOutput = evbuffer_new();
	while (true)
	{
		ErrorCode_t errCode = m_Codec.TryReadMsg(evbuf, pChannel, evOutput, PeerType_t::worldserver);
		if (errCode == ErrorCode_t::eNoMoreData) {
			break;
		}
		else if (errCode == ErrorCode_t::eInvalidLength) {
			pChannel->ForceClose(); //Log Why Error
			break;
		}
		else
		{
			evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
			//Log ?
			continue;
		}
	}
	evbuffer_free(evOutput);
}


void NetManagerWorld::SendPacket(PeerType_t eDestPeer, google::protobuf::Message& packet) {
	PeerInfo pi(eDestPeer);
	m_Codec.SendMsg(m_pConnector, packet, pi.serialize());
}

void NetManagerWorld::OnCenterServer_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
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

void NetManagerWorld::Register(int32_t peer_info)
{
	PktRegisterServer pkt;
	pkt.set_server_info(peer_info);
	SendPacket(PeerType_t::centerserver, pkt);
	//m_pConnector->SendPacket(PeerType_t::worldserver, pkt);
}

void NetManagerWorld::SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet)
{
	m_Codec.SendMsg(channel, packet, channel->GetPeerInfo());
}
void NetManagerWorld::OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt)
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(pi.peer_type == PeerType_t::worldserver);
	assert(pi.peer_index > 0);
	assert(pi.channel_index != 0 && channel->GetChannelIndex() == 0);
	std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << pi.channel_index << std::endl;
	channel->SetPeerInfo(server_info);
}