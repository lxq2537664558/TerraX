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
		MsgHeader msgHeader;
		ErrorCode_t errCode = m_Codec.ReadMessage_OneByOne(evbuf, evOutput, msgHeader);
		if (errCode == ErrorCode_t::eNoMoreData) {
			break;
		}
		if (errCode == ErrorCode_t::eNoError) {
			char* pMsgName = nullptr;
			int nMsgNameSize = 0;
			char* pBuffer = nullptr;
			int nBufferSize = 0;
			ErrorCode_t eParError = m_Codec.Parse(evOutput, msgHeader, pMsgName, nMsgNameSize, pBuffer, nBufferSize);
			if (eParError == ErrorCode_t::eNoError)
			{
				PeerInfo pi;
				pi.parse(msgHeader.dest_peer_info());
				if (pi.peer_type == PeerType_t::centerserver)
				{
					std::string packetname(pMsgName, nMsgNameSize - 1);
					pChannel->OnMessage(msgHeader.from_peer_info(), packetname, pBuffer, nBufferSize);
				}
				else
				{
					NetChannelPtr pFrontEndChannel = (pi.peer_type == PeerType_t::client) ?
						m_pAcceptor->GetChannel(PeerType_t::gateserver, pi.peer_index) : m_pAcceptor->GetChannel(pi.channel_index);
					assert(pFrontEndChannel);
					if (pFrontEndChannel) {
						pChannel->SendMsg(evOutput);
					}
				}
			}
			evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
			continue;
		}
		if (errCode == ErrorCode_t::eInvalidLength) {
			pChannel->ForceClose(); //Log Why Error
			break;
		}
	}
	evbuffer_free(evOutput);
}

void NetManagerCenter::SendPacket(int32_t nDestPeerInfo, google::protobuf::Message& packet) {
	//SendPacket(m_pConnector, nDestPeerInfo, packet);
}

void NetManagerCenter::SendPacket(NetChannelPtr& channel, int32_t nDestPeerInfo, gpb::Message& packet) {
	MsgHeader msgHeader(channel->GetPeerInfo(), nDestPeerInfo);
	struct evbuffer* buf = evbuffer_new();
	m_Codec.Serialize(buf, msgHeader, packet);
	channel->SendMsg(buf);
	evbuffer_free(buf);
}

void NetManagerCenter::SendPacket(int32_t nFromChannelPeer, int32_t nDestPeerInfo, gpb::Message& packet) {
	PeerInfo pi;
	pi.parse(nFromChannelPeer);
	auto& pChannel = m_pAcceptor->GetChannel(pi.channel_index);
	assert(pChannel);
	if (pChannel) {
		SendPacket(pChannel, nDestPeerInfo, packet);
	}
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
