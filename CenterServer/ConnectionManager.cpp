#include "PacketProcessor_Center.h"
#include "CenterServer.h"
#include "ConnectionManager.h"
#include "ServerManager_Center.h"
#include "proto/server_server.pb.h"
using namespace TerraX;
using namespace S2SPacket;

ConnectionManager::ConnectionManager()
{
    RegPacketHandler_Arg3(PktRegisterReq,
                          std::bind(&ConnectionManager::OnMessage_PktRegisterReq, this, std::placeholders::_1,
                                    std::placeholders::_2, std::placeholders::_3));

    for (uint8_t i = 1; i <= MAX_GATE_CONNECTION_COUNT; ++i) {
        m_queue_gateconnIds.push(i);
    }
    for (uint8_t i = 1; i <= MAX_GAME_CONNECTION_COUNT; ++i) {
        m_queue_gameconnIds.push(i);
    }
    for (uint8_t i = 1; i <= MAX_WORLD_CONNECTION_COUNT; ++i) {
        m_queue_worldconnIds.push(i);
    }
}

uint8_t ConnectionManager::GetAvailableConnIdx(PeerType_t peer_type)
{
    switch (peer_type) {
        case PeerType_t::worldserver: {
            if (!m_queue_worldconnIds.empty()) {
                uint8_t conn_id = m_queue_worldconnIds.front();
                m_queue_worldconnIds.pop();
                return conn_id;
            }
            return 0;
        }
        case PeerType_t::gameserver: {
            if (!m_queue_gameconnIds.empty()) {
                uint8_t conn_id = m_queue_gameconnIds.front();
                m_queue_gameconnIds.pop();
                return conn_id;
            }
            return 0;
        }
        case PeerType_t::gateserver: {
            if (!m_queue_gateconnIds.empty()) {
                uint8_t conn_id = m_queue_gateconnIds.front();
                m_queue_gateconnIds.pop();
                return conn_id;
            }
            return 0;
        }
        default:
            break;
    }
    return 0;
}

void ConnectionManager::OnMessage_PktRegisterReq(int32_t nChannelInfo, int32_t nSrcPeerInfo, PktRegisterReq* pkt)
{
    PeerInfo pi_src(nSrcPeerInfo);
    assert(nChannelInfo != 0 && pi_src.channel_index == 0);
    assert(pi_src.peer_index == 0);
    assert(pi_src.peer_type > PeerType_t::client && pi_src.peer_type < PeerType_t::peer_count);
    uint8_t conn_id = GetAvailableConnIdx(pi_src.peer_type);

    NetChannelPtr pChannel = PacketProcessor_Center::GetInstance().GetChannel_FrontEndbyChannelIndex(nChannelInfo);
    if (!pChannel) {
        // failed
        return;
    }
    if (conn_id == 0) {
        pChannel->ForceClose();
    } else {
        pi_src.peer_index = conn_id;
        pi_src.channel_index = 0;
        int32_t reg_peer_info = pi_src.serialize();
		pChannel->SetPeerType(pi_src.peer_type);
		pChannel->SetPeerIndex(conn_id);
        std::cout << "Server: " << pi_src.server_name() << "\t PeerIndex: " << int32_t(pi_src.peer_index)
                  << "\t ChannelIndex: " << pi_src.channel_index << std::endl;

        PktRegisterAck pktAck;
        pktAck.set_server_info(reg_peer_info);
        PacketProcessor_Center::GetInstance().SendPacket(reg_peer_info, nSrcPeerInfo, reg_peer_info, pktAck);
        
		ServerManager_Center::GetInstance().OnServerAdded(reg_peer_info);
    }
}

void ConnectionManager::OnChannel_DisConnect(NetChannelPtr& channel)
{
    switch ((PeerType_t)channel->GetPeerType()) {
        case PeerType_t::worldserver: {
            m_queue_worldconnIds.push(channel->GetPeerIndex());
            return;
        }
        case PeerType_t::gameserver: {
            m_queue_gameconnIds.push(channel->GetPeerIndex());
            return;
        }
        case PeerType_t::gateserver: {
            m_queue_gateconnIds.push(channel->GetPeerIndex());
            return;
        }
        default:
            break;
    }
}