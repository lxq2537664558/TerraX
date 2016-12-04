#include "NetManagerCenter.h"
//#include "GuestManager.h"
#include "ConnectionManager.h"
using namespace TerraX;

NetManagerCenter::NetManagerCenter() {
}

void NetManagerCenter::Accept(int port, uint16_t max_connections) {
	m_pAcceptor.reset(new NetServer(&m_loop, port, max_connections));
	m_pAcceptor->RegNetEvent_Callback(std::bind(&NetManagerCenter::OnAcceptor_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
	m_pAcceptor->RegOnMessage_Callback(std::bind(&NetManagerCenter::OnMessageAcceptor, this, std::placeholders::_1, std::placeholders::_2));
}

void NetManagerCenter::OnMessageAcceptor(evbuffer* evbuf, NetChannelPtr& pChannel)
{
	struct evbuffer* evOutput = evbuffer_new();
	while (true)
	{
		ErrorCode_t errCode = m_Codec.TryReadMsg(evbuf, pChannel, evOutput, PeerType_t::centerserver);
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
			NetChannelPtr m_pChannel = (pi.peer_type == PeerType_t::client) ? 
				m_pAcceptor->GetChannel(PeerType_t::gateserver, pi.peer_index) : m_pAcceptor->GetChannel(pi.channel_index);
			if (m_pChannel) {
				m_pChannel->SendMsg(evOutput);
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

void NetManagerCenter::SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet) {
	m_Codec.SendMsg(channel, packet, channel->GetPeerInfo());
}

void NetManagerCenter::SendPacket(int32_t nDestPeerInfo, google::protobuf::Message& packet) {
	//PeerInfo pi(eDestPeer);
	//m_Codec.SendMsg(m_pConnector, packet, pi.serialize());
}

void NetManagerCenter::OnAcceptor_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if (eEvent == NetEvent_t::eConnected) {
		//Register(channel->GetPeerInfo());
	}
	else if (eEvent == NetEvent_t::eConnectFailed) {
		// do exit...
	}
	else if (eEvent == NetEvent_t::eDisconnected) {
		ConnectionManager::GetInstance().OnChannel_DisConnect(channel);
	}
	else {
		// unknown event
	}
}
