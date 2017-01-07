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
	m_pBackEnd->RegOnMessage_Callback(
		std::bind(&NetManagerClient::OnMessage_BackEnd, this, std::placeholders::_1, std::placeholders::_2));
	m_pBackEnd->RegNetEvent_Callback(
		std::bind(&NetManagerClient::OnNetEvent_BackEnd, this, std::placeholders::_1, std::placeholders::_2));
}


void NetManagerClient::ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, Packet* pkt)
{
}

void NetManagerClient::OnMessage_BackEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
	ProcessMessage(evbuf, pChannel, m_pktQueueBackEnd, nullptr);
}

void NetManagerClient::ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, PacketQueue& pktQueue,
	std::function<void(NetChannelPtr&, Packet*)> fn)
{
	MessageError_t errCode = ReadMessage(evbuf, pktQueue);
	if (errCode == MessageError_t::eInvalidLength) {
		// Log;
		// close channel;
	}
	while (!pktQueue.IsEmpty()) {
		std::unique_ptr<Packet> pkt(pktQueue.Pop());
		if (!pkt->IsValid()) {
			continue;
		}
		std::string packet_name = pkt->GetPacketName();
		pChannel->OnMessage(pkt->GetOwnerInfo(), packet_name, pkt->GetPacketMsg(), pkt->GetMsgSize());
	}
}
MessageError_t NetManagerClient::ReadMessage(struct evbuffer* evbuf, PacketQueue& pktQueue)
{
	std::size_t readable = evbuffer_get_length(evbuf);
	int32_t min_msg_length = Packet::HEADER_SIZE;
	MessageError_t err = MessageError_t::eNoError;
	while (readable >= static_cast<std::size_t>(min_msg_length)) {
		int be32 = 0;
		evbuffer_copyout(evbuf, &be32, sizeof(be32));
		int total_len = ntohl(be32);
		if (total_len > MAX_MESSAGE_SIZE || total_len < min_msg_length) {
			err = MessageError_t::eInvalidLength;
			break;
		}
		else if (readable >= static_cast<std::size_t>(total_len)) {
			Packet* pkt = new Packet(total_len);
			evbuffer_remove(evbuf, pkt->GetBuffer(), total_len);
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
	std::unique_ptr<Packet> pkt(new Packet(msg));
	pkt->SetDestination(pi.serialize());
	m_pBackEnd->SendMsg(pkt->GetBuffer(), pkt->Size());
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