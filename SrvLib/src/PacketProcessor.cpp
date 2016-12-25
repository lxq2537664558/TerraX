#include "PacketProcessor.h"
#include <functional>
#include <event2/bufferevent.h>
#include <event2/buffer.h>

using namespace TerraX;

PacketProcessor::PacketProcessor(PeerType_t peer) : m_peer_type(peer) {}

void PacketProcessor::Tick() { m_loop.loop(); }

void PacketProcessor::Connect(const std::string& host, int port)
{
    m_pBackEnd = std::make_shared<NetChannel>(&m_loop, host, port);
    m_pBackEnd->SetPeerInfo(uint8_t(m_peer_type) << 24);
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

void PacketProcessor::ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, PacketQueue& pktQueue,
                                     std::function<void(Packet*)> fn)
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
        int peer_info = pkt->GetDesination();
        PeerInfo pi(peer_info);
        if (m_peer_type == pi.peer_type) {
            std::string packet_name = pkt->GetPacketName();
            pChannel->OnMessage(pkt->GetOwnerInfo(), packet_name, pkt->GetPacketMsg(), pkt->GetMsgSize());
        } else {
            fn(pkt.get());
        }
    }
}

void PacketProcessor::OnMessage_FrontEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
    ProcessMessage(evbuf, pChannel, m_pktQueueFrontEnd,
                   std::bind(&PacketProcessor::ForwardPacket_BackEnd, this, std::placeholders::_1));
}

void PacketProcessor::OnMessage_BackEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
    ProcessMessage(evbuf, pChannel, m_pktQueueBackEnd,
                   std::bind(&PacketProcessor::ForwardPacket_FrontEnd, this, std::placeholders::_1));
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

NetChannelPtr PacketProcessor::GetChannel_FrontEnd(int32_t nChannelInfo)
{
	PeerInfo pi(nChannelInfo);
	return m_pFrontEnd->GetChannel(pi.channel_index);
}

void PacketProcessor::SendPacket(NetChannelPtr& pChannel, int dest_info, int owner_info, gpb::Message& msg)
{
    std::unique_ptr<Packet> pkt(new Packet(msg));
    pkt->SetDestination(dest_info);
    pkt->SetOwner(owner_info);
    pChannel->SendMsg(pkt->GetBuffer(), pkt->Size());
}


void PacketProcessor::SendPacket2BackEnd(int dest_info, int owner_info, gpb::Message& msg)
{
	SendPacket(m_pBackEnd, dest_info, owner_info, msg);
}