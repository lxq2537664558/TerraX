#include "GatePacketProcessor.h"

using namespace TerraX;
GatePacketProcessor::GatePacketProcessor() : PacketProcessor(PeerType_t::gateserver)
{
    // PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterAck>(
    // new events_dynamic2(std::function<void(PktRegisterAck*)>(
    // std::bind(&GatePacketProcessor::OnMessage_LoginResult, this, std::placeholders::_1))));

    RegPacketHandler_Arg1(
        PktRegisterAck, std::bind(&GatePacketProcessor::OnMessage_LoginResult, this, std::placeholders::_1));
}

void GatePacketProcessor::ForwardPacket_FrontEnd(Packet* pkt) {}

void GatePacketProcessor::ForwardPacket_BackEnd(Packet* pkt) {}

void GatePacketProcessor::OnNetEvent_FrontEnd(NetChannelPtr& pChannel, NetEvent_t eEvent) {}

void GatePacketProcessor::OnNetEvent_BackEnd(NetChannelPtr& pChannel, NetEvent_t eEvent)
{
    switch (eEvent) {
        case NetEvent_t::eConnected:
            Login2Center();
            break;
        case NetEvent_t::eConnectFailed:
            break;
        case NetEvent_t::eDisconnected:
            break;
        default:
            break;
    }
}

void GatePacketProcessor::Login2Center()
{
    PeerInfo pi_dest(PeerType_t::centerserver);
    PeerInfo pi_src(PeerType_t::gateserver);
    PktRegisterReq pkt;
    SendPacket2BackEnd(pi_dest.serialize(), pi_src.serialize(), pkt);
}

void GatePacketProcessor::OnMessage_LoginResult(PktRegisterAck* pkt)
{
    PeerInfo pi(pkt->server_info());
    assert(pi.peer_type == PeerType_t::gateserver);
    assert(pi.peer_index > 0);
    assert(pi.channel_index != 0 && m_pBackEnd->GetChannelIndex() == 0);
    std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index)
              << "\t ChannelIndex: " << pi.channel_index << std::endl;
    m_pBackEnd->SetPeerInfo(pkt->server_info());
}
