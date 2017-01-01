#include "ServerManager.h"

#include "PacketDispatcher.h"
#include "PacketProcessor.h"
#include "proto/server_server.pb.h"
using namespace S2SPacket;
using namespace TerraX;

ServerManager::ServerManager() { RegPacketHanderFunction(); }
void ServerManager::RegPacketHanderFunction()
{
    RegPacketHandler_Arg1(PktServerSync,
                          std::bind(&ServerManager::OnMessage_PktServerSync, this, std::placeholders::_1));
}

void ServerManager::InitPacketProcessor(class PacketProcessor* pPktProcessor)
{
    m_pPktProcessor = pPktProcessor;
}

int ServerManager::GetCenterPeerInfo()
{
	auto it = m_ServerInfos.find(PeerType_t::centerserver);
	if (it != m_ServerInfos.end())
	{
		auto var = it->second;
		if (!var.empty())
		{
			return *(var.begin());
		}
	}
	return 0;
}
int ServerManager::GetWorldPeerInfo()
{
	auto it = m_ServerInfos.find(PeerType_t::worldserver);
	if (it != m_ServerInfos.end())
	{
		auto var = it->second;
		if (!var.empty())
		{
			return *(var.begin());
		}
	}
	return 0;
}

void ServerManager::OnServerAdded(int32_t peer_info)
{
    PeerInfo pi(peer_info);
    switch (pi.peer_type) {
        case PeerType_t::gateserver:
            ProcessGateServerAdded(peer_info);
            break;
        case PeerType_t::centerserver:
            ProcessCenterServerAdded(peer_info);
            break;
        case PeerType_t::worldserver:
            ProcessWorldServerAdded(peer_info);
            break;
        case PeerType_t::gameserver:
            ProcessGameServerAdded(peer_info);
            break;
        default:
            break;
    }
    m_ServerInfos[pi.peer_type].insert(peer_info);
}

void ServerManager::OnServerRemoved(int32_t peer_info)
{
    PeerInfo pi(peer_info);
    switch (pi.peer_type) {
        case PeerType_t::gateserver:
            ProcessGateServerRemoved(peer_info);
            break;
        case PeerType_t::centerserver:
            ProcessCenterServerRemoved(peer_info);
            break;
        case PeerType_t::worldserver:
            ProcessWorldServerRemoved(peer_info);
            break;
        case PeerType_t::gameserver:
            ProcessGameServerRemoved(peer_info);
            break;
        default:
            break;
    }
    m_ServerInfos[pi.peer_type].erase(peer_info);
    if (m_ServerInfos[pi.peer_type].empty()) {
        m_ServerInfos.erase(pi.peer_type);
    }
}

void ServerManager::AddServerInfo2Pkt(PeerType_t peer_type, PktServerSync& pkt)
{
    auto it = m_ServerInfos.find(peer_type);
    if (it != m_ServerInfos.end()) {
        for (auto& var : it->second) {
            pkt.add_server_info(var);
        }
    }
}

void ServerManager::BroadCastGateServerChanged(PktServerSync& pkt)
{
    {
        auto itGameServer = m_ServerInfos.find(PeerType_t::gameserver);
        if (itGameServer != m_ServerInfos.end()) {
            for (auto& var : itGameServer->second) {
                m_pPktProcessor->SendPacket2FrontEnd(var, var, 0, pkt);
            }
        }
    }
    {
        auto itWorldServer = m_ServerInfos.find(PeerType_t::worldserver);
        if (itWorldServer != m_ServerInfos.end()) {
            for (auto& var : itWorldServer->second) {
                m_pPktProcessor->SendPacket2FrontEnd(var, var, 0, pkt);
            }
        }
    }
}
void ServerManager::BroadCastWorldServerChanged(PktServerSync& pkt)
{
    {
        auto itGameServer = m_ServerInfos.find(PeerType_t::gameserver);
        if (itGameServer != m_ServerInfos.end()) {
            for (auto& var : itGameServer->second) {
                m_pPktProcessor->SendPacket2FrontEnd(var, var, 0, pkt);
            }
        }
    }
    {
        auto itWorldServer = m_ServerInfos.find(PeerType_t::gateserver);
        if (itWorldServer != m_ServerInfos.end()) {
            for (auto& var : itWorldServer->second) {
                m_pPktProcessor->SendPacket2FrontEnd(var, var, 0, pkt);
            }
        }
    }
}
void ServerManager::BroadCastGameServerChanged(PktServerSync& pkt)
{
	{
		auto itGameServer = m_ServerInfos.find(PeerType_t::gateserver);
		if (itGameServer != m_ServerInfos.end()) {
			for (auto& var : itGameServer->second) {
				m_pPktProcessor->SendPacket2FrontEnd(var, var, 0, pkt);
			}
		}
	}
	{
        auto itWorldServer = m_ServerInfos.find(PeerType_t::worldserver);
        if (itWorldServer != m_ServerInfos.end()) {
            for (auto& var : itWorldServer->second) {
                m_pPktProcessor->SendPacket2FrontEnd(var, var, 0, pkt);
            }
        }
    }
}

void ServerManager::BroadCastServerInfoChanged(PeerType_t peer_type, int32_t peer_info,
                                               PktServerSync_SyncType eSyncType)
{
    PktServerSync pktSync;
    pktSync.set_type(eSyncType);
    pktSync.add_server_info(peer_info);
    switch (peer_type) {
        case TerraX::PeerType_t::gateserver:
            BroadCastGateServerChanged(pktSync);
            break;
        case TerraX::PeerType_t::gameserver:
            BroadCastGameServerChanged(pktSync);
            break;
        case TerraX::PeerType_t::worldserver:
            BroadCastWorldServerChanged(pktSync);
            break;
        default:
            break;
    }
}

void ServerManager::ProcessGateServerAdded(int32_t peer_info)
{
    PktServerSync pktAdd;
    pktAdd.set_type(PktServerSync_SyncType_add);
    AddServerInfo2Pkt(PeerType_t::centerserver, pktAdd);
    AddServerInfo2Pkt(PeerType_t::worldserver, pktAdd);
    AddServerInfo2Pkt(PeerType_t::gameserver, pktAdd);
    m_pPktProcessor->SendPacket2FrontEnd(peer_info, peer_info, 0, pktAdd);

    BroadCastServerInfoChanged(PeerType_t::gateserver, peer_info, PktServerSync_SyncType_add);
}
void ServerManager::ProcessCenterServerAdded(int32_t peer_info)
{
    // log
}
void ServerManager::ProcessWorldServerAdded(int32_t peer_info)
{
    PktServerSync pktAdd;
    pktAdd.set_type(PktServerSync_SyncType_add);
    AddServerInfo2Pkt(PeerType_t::centerserver, pktAdd);
    AddServerInfo2Pkt(PeerType_t::gateserver, pktAdd);
    AddServerInfo2Pkt(PeerType_t::gameserver, pktAdd);
    m_pPktProcessor->SendPacket2FrontEnd(peer_info, peer_info, 0, pktAdd);

    BroadCastServerInfoChanged(PeerType_t::worldserver, peer_info, PktServerSync_SyncType_add);
}
void ServerManager::ProcessGameServerAdded(int32_t peer_info)
{
    PktServerSync pktAdd;
    pktAdd.set_type(PktServerSync_SyncType_add);
    AddServerInfo2Pkt(PeerType_t::centerserver, pktAdd);
    AddServerInfo2Pkt(PeerType_t::gateserver, pktAdd);
    AddServerInfo2Pkt(PeerType_t::worldserver, pktAdd);
    m_pPktProcessor->SendPacket2FrontEnd(peer_info, peer_info, 0, pktAdd);

    BroadCastServerInfoChanged(PeerType_t::gameserver, peer_info, PktServerSync_SyncType_add);
}

void ServerManager::ProcessGateServerRemoved(int32_t peer_info)
{
    BroadCastServerInfoChanged(PeerType_t::gateserver, peer_info, PktServerSync_SyncType_remove);
}
void ServerManager::ProcessCenterServerRemoved(int32_t peer_info)
{
    // LOG
}
void ServerManager::ProcessWorldServerRemoved(int32_t peer_info)
{
    BroadCastServerInfoChanged(PeerType_t::worldserver, peer_info, PktServerSync_SyncType_remove);
}
void ServerManager::ProcessGameServerRemoved(int32_t peer_info)
{
    BroadCastServerInfoChanged(PeerType_t::gameserver, peer_info, PktServerSync_SyncType_remove);
}

void ServerManager::OnMessage_PktServerSync(S2SPacket::PktServerSync* pkt)
{
    if (pkt->type() == PktServerSync_SyncType_add || pkt->type() == PktServerSync_SyncType_update) {
        for (int i = 0; i < pkt->server_info_size(); ++i) {
            PeerInfo pi(pkt->server_info(i));
            m_ServerInfos[pi.peer_type].insert(pkt->server_info(i));
            std::cout << "Add Server : \t" << pi.server_name() << std::ends << (int32_t)pi.peer_index
                      << std::ends << pi.channel_index << std::endl;
        }
    }
    if (pkt->type() == PktServerSync_SyncType_remove) {
        for (int i = 0; i < pkt->server_info_size(); ++i) {
            PeerInfo pi(pkt->server_info(i));
            auto it = m_ServerInfos.find(pi.peer_type);
            assert(it != m_ServerInfos.end());
            if (it != m_ServerInfos.end()) {
                (it->second).erase(pkt->server_info(i));
                std::cout << "Remove Server : \t" << pi.server_name() << std::ends << (int32_t)pi.peer_index << std::ends
                          << pi.channel_index << std::endl;
            }
        }
    }
}