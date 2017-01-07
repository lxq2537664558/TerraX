#include "CenterServer.h"
#include "ConnectionManager.h"
#include "PacketProcessor_Center.h"
#include "ServerManager_Center.h"
#include "proto/server_server.pb.h"
using namespace TerraX;
using namespace S2SPacket;

ConnectionManager::ConnectionManager()
{
    REG_PACKET_HANDLER_ARG3(
        PktRegisterReq, std::bind(&ConnectionManager::OnMessage_PktRegisterReq, this, std::placeholders::_1,
                                  std::placeholders::_2, std::placeholders::_3));
}

void ConnectionManager::OnMessage_PktRegisterReq(uint16_t channel_index, int32_t nSrcPeerInfo,
                                                 PktRegisterReq* pkt)
{
    PeerInfo pi_src(nSrcPeerInfo);
    assert(channel_index != 0 && pi_src.channel_index == 0);
    assert(pi_src.peer_index == 0);
    assert(pi_src.peer_type > PeerType_t::client && pi_src.peer_type < PeerType_t::peer_count);
    NetChannelPtr pChannel = PacketProcessor_Center::GetInstance().GetChannel_FrontEnd(channel_index);
    if (!pChannel) {
        // failed
        return;
    }
    pChannel->SetPeerType(pi_src.peer_type);

    pi_src.channel_index = channel_index;
    int32_t reg_peer_info = pi_src.serialize();

    std::cout << "Server: " << pi_src.server_name() << "\t PeerIndex: " << int32_t(pi_src.peer_index)
              << "\t ChannelIndex: " << pi_src.channel_index << std::endl;

    PktRegisterAck pktAck;
    pktAck.set_server_info(reg_peer_info);
    PacketProcessor_Center::GetInstance().SendPacket(channel_index, nSrcPeerInfo, reg_peer_info, pktAck);

    ServerManager_Center::GetInstance().OnServerAdded(reg_peer_info);
}
