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
        [this](evbuffer* evbuf, NetChannelPtr& pChannel) { this->OnMessage_BackEnd(evbuf, pChannel); });
    m_pBackEnd->RegNetEvent_Callback(
        [this](NetChannelPtr& pChannel, NetEvent_t eEvent) { this->OnNetEvent_BackEnd(pChannel, eEvent); });
}

void PacketProcessor::Accept(int port, uint16_t max_connections)
{
    m_pFrontEnd.reset(new NetServer(&m_loop, port, max_connections));
    m_pFrontEnd->RegOnMessage_Callback(
        [this](evbuffer* evbuf, NetChannelPtr& pChannel) { this->OnMessage_FrontEnd(evbuf, pChannel); });
    m_pFrontEnd->RegNetEvent_Callback(
        [this](NetChannelPtr& pChannel, NetEvent_t eEvent) { this->OnNetEvent_BackEnd(pChannel, eEvent); });
}

void PacketProcessor::ForwardPacketOnBackEnd(NetChannelPtr& pBackChannel, PacketBase* pkt) {}
void PacketProcessor::ForwardPacketOnFrontEnd(NetChannelPtr& pFrontChannel, PacketBase* pkt) {}

void PacketProcessor::OnMessage_FrontEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
    ProcessMessage(
        evbuf, pChannel, (m_peer_type == PeerType_t::gateserver),
        [this](NetChannelPtr& pChannel, PacketBase* pkt) { this->ForwardPacketOnFrontEnd(pChannel, pkt); });
}

void PacketProcessor::OnMessage_BackEnd(evbuffer* evbuf, NetChannelPtr& pChannel)
{
    ProcessMessage(evbuf, pChannel, false, [this](NetChannelPtr& pChannel, PacketBase* pkt) {
        this->ForwardPacketOnBackEnd(pChannel, pkt);
    });
}

void PacketProcessor::ProcessMessage(evbuffer* evbuf, NetChannelPtr& pChannel, bool bFromClient,
                                     std::function<void(NetChannelPtr&, PacketBase*)> fn)
{
    MessageError_t errCode = ReadMessage(evbuf, bFromClient);
    if (errCode == MessageError_t::eInvalidLength) {
        // Log; // it would never happened. if it happened,
        // we think we should add '$' as the end of message.
        // close channel;
    }
    while (!m_queueRead.IsEmpty()) {
        std::unique_ptr<PacketBase> pkt(m_queueRead.Pop());
        if (!pkt->is_valid()) {
            continue;
        }

        if (fn) {
            fn(pChannel, pkt.get());
        }
    }
}
MessageError_t PacketProcessor::ReadMessage(struct evbuffer* evbuf, bool bFromClient)
{
    std::size_t readable = evbuffer_get_length(evbuf);
    int32_t min_msg_length = sizeof(uint16_t);  // checksum
    MessageError_t err = MessageError_t::eNoError;
    while (readable >= static_cast<std::size_t>(min_msg_length)) {
        uint16_t be16 = 0;
        evbuffer_copyout(evbuf, &be16, sizeof(be16));
        int32_t total_len = static_cast<int32_t>(ntohs(be16));
        if (total_len > MAX_PACKET_SIZE || total_len < min_msg_length) {
            err = MessageError_t::eInvalidLength;
            break;
        } else if (readable >= static_cast<std::size_t>(total_len)) {
            PacketBase* pkt;
            if (bFromClient) {
                pkt = new PacketC(total_len, PacketC::EX_DATA_SIZE);
            } else {
                pkt = new PacketS(total_len);
            }
            evbuffer_remove(evbuf, pkt->buffer(), total_len);
            m_queueRead.Push(pkt);
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

void PacketProcessor::SendPacketByChannel(NetChannelPtr& pChannel, int dest_info, int owner_info,
                                          gpb::Message& msg)
{
    PacketS pkt(msg);
    if (pkt.AppendDestination(dest_info)) {
        pkt.SetOwner(owner_info);
        pChannel->SendMsg(pkt.buffer(), pkt.GetPacketSize());
    }
}

void PacketProcessor::SendPacket2FrontEnd(uint16_t channel_index, int dest_info, int owner_info,
                                          gpb::Message& msg)
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

void PacketProcessor::DoBackEnd_Connected(NetChannelPtr& pChannel) {}

void PacketProcessor::DoBackEnd_Disconnected(NetChannelPtr& pChannel) {}

void PacketProcessor::DoBackEnd_ConnBreak(NetChannelPtr& pChannel) {}

void PacketProcessor::DoFrontEnd_Connected(NetChannelPtr& pChannel) {}
void PacketProcessor::DoFrontEnd_Disconnected(NetChannelPtr& pChannel) {}
void PacketProcessor::DoFrontEnd_ConnBreak(NetChannelPtr& pChannel) {}