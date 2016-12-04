#include "NetManagerGate.h"
#include "GuestManager.h"
using namespace TerraX;

NetManagerGate::NetManagerGate() {
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&NetManagerGate::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2)));
}


void NetManagerGate::Connect(const std::string& host, int port) {
	m_pConnector = std::make_shared<NetChannel>(&m_loop, host, port);
	m_pConnector->SetPeerInfo(uint8_t(PeerType_t::gateserver) << 24);
	m_pConnector->RegNetEvent_Callback(std::bind(&NetManagerGate::OnCenterServer_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	m_pConnector->RegOnMessage_Callback(std::bind(&NetManagerGate::OnMessageCenterServer, this, std::placeholders::_1, std::placeholders::_2));
}

void NetManagerGate::Accept(int port, uint16_t max_connections) {
	m_pAcceptor.reset(new NetServer(&m_loop, port, max_connections));
	m_pAcceptor->RegNetEvent_Callback(std::bind(&NetManagerGate::OnClient_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	m_pAcceptor->RegOnMessage_Callback(std::bind(&NetManagerGate::OnMessageClient, this, std::placeholders::_1, std::placeholders::_2));
}

void NetManagerGate::OnMessageCenterServer(evbuffer* evbuf, NetChannelPtr& pChannel)
{
	struct evbuffer* evOutput = evbuffer_new();
	while (true)
	{
		ErrorCode_t errCode = m_Codec.TryReadMsg(evbuf, pChannel, evOutput, PeerType_t::gateserver);
		if (errCode == ErrorCode_t::eNoMoreData) {
			break;
		}
		else if (errCode == ErrorCode_t::eNotDestination) {
			int nDestPeerInfo = 0;
			if (!m_Codec.TryExtractDestInfo(evOutput, nDestPeerInfo)) {
				evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
				//LOG why Error
				continue;
			}
			PeerInfo pi;
			pi.parse(nDestPeerInfo);
			NetChannelPtr m_pCltChannel = m_pAcceptor->GetChannel(pi.channel_index);
			if (m_pCltChannel) {
				m_pCltChannel->SendMsg(evOutput);
			}
			else {
				evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
			}
		}
		else if (errCode == ErrorCode_t::eInvalidLength) {
			pChannel->ForceClose(); //Log Why Error
			break;
		}
		else
		{
			//Log ?
			evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
			continue;
		}
	}
	evbuffer_free(evOutput);
}

void NetManagerGate::OnMessageClient(evbuffer* evbuf, NetChannelPtr& pChannel)
{
	struct evbuffer* evOutput = evbuffer_new();
	while (true)
	{
		ErrorCode_t errCode = m_Codec.TryReadMsg(evbuf, pChannel, evOutput, PeerType_t::gateserver);
		if (errCode == ErrorCode_t::eNoMoreData) {
			break;
		}
		else if ( errCode == ErrorCode_t::eNotDestination) {
			int nDestPeerInfo = 0;
			if (!m_Codec.TryExtractDestInfo(evOutput, nDestPeerInfo)) {
				evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
				//LOG why Error
				continue;
			}
			const Guest* pGuest = GuestManager::GetInstance().FindGuest(pChannel->GetPeerInfo());
			if (!pGuest) {
				evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
				continue;
			}
			PeerInfo pi;
			pi.parse(nDestPeerInfo);
			if (m_Codec.TrySetMsgInfo(evOutput, pGuest->GetDestPeerInfo(pi.peer_type), pGuest->GetGuestID())) {
					m_pConnector->SendMsg(evOutput);
			}
			else {
				evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
			}
		}
		else if (errCode == ErrorCode_t::eInvalidLength) {
			pChannel->ForceClose(); //Log Why Error
			break;
		}
		else
		{
			//Log ?
			continue;
		}
	}
	evbuffer_free(evOutput);
}

void NetManagerGate::SendPacket(PeerType_t eDestPeer, google::protobuf::Message& packet) {
	PeerInfo pi(eDestPeer);
	m_Codec.SendMsg(m_pConnector, packet, pi.serialize());
}

void NetManagerGate::OnClient_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if (eEvent == NetEvent_t::eConnected) {
		channel->SetPeerType(PeerType_t::client);
		channel->SetPeerIndex(m_pConnector->GetPeerIndex());
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
	SendPacket(PeerType_t::centerserver, pkt);
	//m_pConnector->SendPacket(PeerType_t::worldserver, pkt);
}

void NetManagerGate::SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet)
{
	m_Codec.SendMsg(channel, packet, channel->GetPeerInfo());
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