#include <event2/buffer.h>
#include <event2/bufferevent.h>
#include <functional>
#include "PacketProcessor.h"

using namespace TerraX;

PacketProcessor::PacketProcessor(PeerType_t peer) : m_peer_type(peer) {}

void PacketProcessor::Tick() { m_loop.loop(); }

void PacketProcessor::Connect(const std::string& host, int port)
{
    m_pBackEnd = std::make_shared<NetChannel>(&m_loop, host, port);
    m_pBackEnd->SetPeerType(PeerType_t(m_peer_type));
    m_pBackEnd->RegOnMessage_Callback(
        std::bind(&PacketProcessor::OnMessage_BackEnd, this, std::placeholders::_1, std::placeholders::_2));
    m_pBackEnd->RegNetEvent_Callback(
        std::bind(&PacketProcessor::OnNetEvent_BackEnd, this, std::placeholders::_1, std::placeholders::_2));
}

void PacketProcessor::Accept(int port, uint16_t max_connections)
{
    m_pFrontEnd.reset(new NetServer(&m_loop, port, max_connections));
    m_pFrontEnd->RegOnMessage_Callback(
        std::bind(&PacketProcessor::OnMessage_FrontEnd, this, std::placeholders::_1, std::placeholders::_2));
    m_pFrontEnd->RegNetEvent_Callback(
        std::bind(&PacketProcessor::OnNetEvent_FrontEnd, this, std::placeholders::_1, std::placeholders::_2));
}


void PacketProcessor::ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, Packet* pkt)
{

}
void PacketProcessor::ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, Packet* pkt)
{
}

void PacketProcessor::OnMessage_FrontEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
    ProcessMessage(evbuf, pChannel, m_pktQueueFrontEnd,
                   std::bind(&PacketProcessor::ForwardPacketOnFrontEnd, this, std::placeholders::_1,
                             std::placeholders::_2));
}

void PacketProcessor::OnMessage_BackEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
    ProcessMessage(evbuf, pChannel, m_pktQueueBackEnd,
		std::bind(&PacketProcessor::ForwardPacketOnBackEnd, this, std::placeholders::_1,
			std::placeholders::_2));
}

void PacketProcessor::ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, PacketQueue& pktQueue,
	std::function<void(NetChannelPtr&, Packet*)> fn)
{
	MessageError_t errCode = ReadMessage(evbuf, pktQueue);
	if (errCode == MessageError_t::eInvalidLength) {
		// Log; // it would never happened. if it happened, 
		// we think we should add '$' as the end of message.
		// close channel;
	}
	while (!pktQueue.IsEmpty()) {
		std::unique_ptr<Packet> pkt(pktQueue.Pop());
		if (!pkt->IsValid()) {
			continue;
		}

		if (fn) {
			fn(pChannel, pkt.get());
		}
	}
}
MessageError_t PacketProcessor::ReadMessage(struct evbuffer* evbuf, PacketQueue& pktQueue)
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
        } else if (readable >= static_cast<std::size_t>(total_len)) {
            Packet* pkt = new Packet(total_len);
            evbuffer_remove(evbuf, pkt->GetBuffer(), total_len);
            pktQueue.Push(pkt);
            readable = evbuffer_get_length(evbuf);
            continue;
        } else {
            break;
        }
    }
    return err;
}

NetChannelPtr PacketProcessor::GetChannel_FrontEnd(int16_t channel_index)
{
	return m_pFrontEnd->GetChannel(channel_index);
}

void PacketProcessor::SendPacketByChannel(NetChannelPtr& pChannel, int dest_info, int owner_info, gpb::Message& msg)
{
    std::unique_ptr<Packet> pkt(new Packet(msg));
    pkt->SetDestination(dest_info);
    pkt->SetOwner(owner_info);
    pChannel->SendMsg(pkt->GetBuffer(), pkt->Size());
}

void PacketProcessor::SendPacket2FrontEnd(uint16_t channel_index, int dest_info, int owner_info, gpb::Message& msg)
{
    NetChannelPtr pChannel = GetChannel_FrontEnd(channel_index);
    if (!pChannel) {
        return;
    }
    SendPacketByChannel(pChannel, dest_info, owner_info, msg);
}

void PacketProcessor::SendPacket2BackEnd(int dest_info, int owner_info, gpb::Message& msg)
{
    SendPacketByChannel(m_pBackEnd, dest_info, owner_info, msg);
}

void PacketProcessor::OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent)
{
	switch (eEvent) {
	case TerraX::NetEvent_t::eConnected:
		DoFrontEnd_Connected(pChannel);
		break;
	case TerraX::NetEvent_t::eConnectFailed:
		DoFrontEnd_ConnBreak(pChannel);
		break;
	case TerraX::NetEvent_t::eDisconnected:
		DoFrontEnd_Disconnected(pChannel);
		break;
	default:
		break;
	}
}

void PacketProcessor::OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent)
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

void PacketProcessor::DoBackEnd_Connected(NetChannelPtr& pChannel)
{

}

void PacketProcessor::DoBackEnd_Disconnected(NetChannelPtr& pChannel)
{

}

void PacketProcessor::DoBackEnd_ConnBreak(NetChannelPtr& pChannel)
{

}

void PacketProcessor::DoFrontEnd_Connected(NetChannelPtr& pChannel)
{

}
void PacketProcessor::DoFrontEnd_Disconnected(NetChannelPtr& pChannel)
{

}
void PacketProcessor::DoFrontEnd_ConnBreak(NetChannelPtr& pChannel)
{

}