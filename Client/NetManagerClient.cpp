#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <functional>
#include "NetManagerClient.h"
#include "GameStateManager.h"
using namespace TerraX;

NetManagerClient::NetManagerClient() : m_peer_type(PeerType_t::client) {}

void NetManagerClient::Tick() { m_loop.loop(); }

void NetManagerClient::Connect(const std::string& host, int port)
{
	m_pBackEnd = std::make_shared<NetChannel>(&m_loop, host, port);
	m_pBackEnd->SetPeerType(PeerType_t(m_peer_type));
	m_pBackEnd->RegOnMessage_Callback([this](evbuffer* evbuf, NetChannelPtr& pChannel) { this->OnMessage_BackEnd(evbuf, pChannel); });
	m_pBackEnd->RegNetEvent_Callback([this](NetChannelPtr& pChannel, NetEvent_t eEvent) { this->OnNetEvent_BackEnd(pChannel, eEvent); });
}

void NetManagerClient::OnMessage_BackEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
	ProcessMessage(evbuf, pChannel, m_pktQueueBackEnd, nullptr);
}

void NetManagerClient::ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, PacketQueue& pktQueue,
	std::function<void(NetChannelPtr&, PacketC*)> fn)
{
	MessageError_t errCode = ReadMessage(evbuf, pktQueue);
	if (errCode == MessageError_t::eInvalidLength) {
		// Log;
		// close channel;
	}
	while (!pktQueue.IsEmpty()) {
		std::unique_ptr<PacketC> pkt(static_cast<PacketC*>(pktQueue.Pop()));
		if (!pkt->is_valid()) {
			continue;
		}
		std::string packet_name = pkt->GetPacketName();
		pChannel->OnMessage(pkt->GetOwnerInfo(), packet_name, pkt->GetPacketMsg(), pkt->GetMsgSize());
	}
}
MessageError_t NetManagerClient::ReadMessage(struct evbuffer* evbuf, PacketQueue& pktQueue)
{
	std::size_t readable = evbuffer_get_length(evbuf);
	int32_t min_msg_length = sizeof(uint16_t);
	MessageError_t err = MessageError_t::eNoError;
	while (readable >= static_cast<std::size_t>(min_msg_length)) {
		uint16_t be16 = 0;
		evbuffer_copyout(evbuf, &be16, sizeof(be16));
		int total_len = ntohs(be16);
		if (total_len > MAX_PACKET_SIZE || total_len < min_msg_length) {
			err = MessageError_t::eInvalidLength;
			break;
		}
		else if (readable >= static_cast<std::size_t>(total_len)) {
			PacketBase* pkt = new PacketC(total_len);
			evbuffer_remove(evbuf, pkt->buffer(), total_len);
			pktQueue.Push(pkt);
			readable = evbuffer_get_length(evbuf);
			continue;
		}
		else {
			break;
		}
	}
	return err;
}

void NetManagerClient::SendPacket(PeerType_t peer_type, gpb::Message& msg)
{
	PeerInfo pi(peer_type);
	std::unique_ptr<PacketC> pkt(new PacketC(msg));
	pkt->SetOwner(pi.serialize());
	m_pBackEnd->SendMsg(pkt->buffer(), pkt->capacity());
}



void NetManagerClient::OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent)
{
	switch (eEvent) {
	case TerraX::NetEvent_t::eConnected:
		DoBackEnd_Connected(pChannel);
		break;
	case TerraX::NetEvent_t::eConnectFailed:
		DoBackEnd_ConnBreak(pChannel);
		break;
	case TerraX::NetEvent_t::eDisconnected:
		DoBackEnd_Disconnected(pChannel);
		break;
	default:
		break;
	}
}

void NetManagerClient::DoBackEnd_Connected(NetChannelPtr& pChannel)
{
	GameStateManager::GetInstance().NextState(GameState_t::eAccountCheckingPermission);
}

void NetManagerClient::DoBackEnd_Disconnected(NetChannelPtr& pChannel)
{
	std::cout << "connection exit normally!" << std::endl;
}

void NetManagerClient::DoBackEnd_ConnBreak(NetChannelPtr& pChannel)
{
	std::cout << "connection break!" << std::endl;
}