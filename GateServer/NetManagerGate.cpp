#include "NetManagerGate.h"
#include "GuestManager.h"
using namespace TerraX;

NetManagerGate::NetManagerGate() {
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&NetManagerGate::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3)));
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
				if (pi.peer_type == PeerType_t::gateserver)
				{
					std::string packetname(pMsgName, nMsgNameSize - 1);
					pChannel->OnMessage(msgHeader.from_peer_info(), packetname, pBuffer, nBufferSize);
				}
				else
				{
					auto& pFrontEndChannel = m_pAcceptor->GetChannel(pi.channel_index);
					assert(pFrontEndChannel);
					if (pFrontEndChannel) {
						pFrontEndChannel->SendMsg(evOutput);
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

void NetManagerGate::OnMessageClient(evbuffer* evbuf, NetChannelPtr& pChannel)
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
			ErrorCode_t eParError =  m_Codec.Parse(evOutput, msgHeader, pMsgName, nMsgNameSize, pBuffer, nBufferSize);
			if (eParError == ErrorCode_t::eNoError)
			{
				PeerInfo pi;
				pi.parse(msgHeader.dest_peer_info());
				if (pi.peer_type == PeerType_t::gateserver)
				{
					std::string packetname(pMsgName, nMsgNameSize - 1);
					pChannel->OnMessage(pChannel->GetPeerInfo(), packetname, pBuffer, nBufferSize);
				}
				else
				{
					const Guest* pGuest = GuestManager::GetInstance().FindGuest(pChannel->GetPeerInfo());
					if (!pGuest) {
						evbuffer_drain(evOutput, evbuffer_get_length(evOutput));
						continue;
					}
					msgHeader.set_dest_peer_info(pGuest->GetDestPeerInfo(pi.peer_type));
					msgHeader.set_from_peer_info(pChannel->GetPeerInfo());
					m_Codec.SetMsgHeader(evOutput, msgHeader);
					m_pConnector->SendMsg(evOutput);
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
	PeerInfo pi(PeerType_t::centerserver);
	PktRegisterServer pkt;
	pkt.set_server_info(peer_info);
	SendPacket2Backend(pi.serialize(), pkt);
	//m_pConnector->SendPacket(PeerType_t::worldserver, pkt);
}

void NetManagerGate::SendPacket2Backend(int32_t nDestPeerInfo, google::protobuf::Message& packet)
{
	MsgHeader msgHeader(nDestPeerInfo, m_pConnector->GetPeerInfo());
	struct evbuffer* buf = evbuffer_new();
	m_Codec.Serialize(buf, msgHeader, packet);
	m_pConnector->SendMsg(buf);
	evbuffer_free(buf);
}

void NetManagerGate::SendPacket(NetChannelPtr& channel, google::protobuf::Message& packet)
{
	//m_Codec.SendMsg(channel, packet, channel->GetPeerInfo());
}
void NetManagerGate::OnMessage_RegisterResult(NetChannelPtr& channel, int32_t nFromPeerInfo, PktRegisterServer& pkt)
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