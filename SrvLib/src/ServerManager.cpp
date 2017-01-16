#include "ServerManager.h"

#include "PacketDispatcher.h"
#include "PacketProcessor.h"
#include "proto/server_server.pb.h"
using namespace S2SPacket;
using namespace TerraX;

ServerManager::ServerManager() { RegPacketHanderFunction(); }
void ServerManager::RegPacketHanderFunction()
{
	REG_PACKET_HANDLER_ARG1(PktServerSync, this, OnMessage_PktServerSync);
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

void ServerManager::OnMessage_PktServerSync(PktServerSync* pkt)
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