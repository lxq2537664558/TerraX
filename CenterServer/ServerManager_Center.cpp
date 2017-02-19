#include "ServerManager_Center.h"

#include "PacketDispatcher.h"
#include "PacketProcessor_Center.h"

using namespace S2SPacket;
using namespace TerraX;

void ServerManager_Center::InitPacketProcessor(PacketProcessor_Center* pPktProcessor)
{
	m_pPktProcessor = pPktProcessor;
}

void ServerManager_Center::OnServerAdded(int32_t peer_info)
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

void ServerManager_Center::OnServerRemoved(int32_t peer_info)
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

void ServerManager_Center::AddServerInfo2Pkt(PeerType_t peer_type, PktServerSync& pkt)
{
	auto it = m_ServerInfos.find(peer_type);
	if (it != m_ServerInfos.end()) {
		for (auto&& var : it->second) {
			pkt.add_server_info(var);
		}
	}
}

void ServerManager_Center::BroadCastGateServerChanged(PktServerSync& pkt)
{
	SendPacket2Server(pkt, PeerType_t::gameserver);
	SendPacket2Server(pkt, PeerType_t::worldserver);
}
void ServerManager_Center::BroadCastWorldServerChanged(PktServerSync& pkt)
{
	SendPacket2Server(pkt, PeerType_t::gameserver);
	SendPacket2Server(pkt, PeerType_t::gateserver);
}
void ServerManager_Center::BroadCastGameServerChanged(PktServerSync& pkt)
{
	SendPacket2Server(pkt, PeerType_t::worldserver);
	SendPacket2Server(pkt, PeerType_t::gateserver);
}

void ServerManager_Center::SendPacket2Server(S2SPacket::PktServerSync& pkt, PeerType_t ePeerType)
{
	auto it = m_ServerInfos.find(ePeerType);
	if (it != m_ServerInfos.end()) {
		for (auto&& var : it->second) {
			m_pPktProcessor->SendPacket(PeerInfo::GetChannelIndex(var), var, 0, pkt);
		}
	}
}

void ServerManager_Center::BroadCastServerInfoChanged(PeerType_t peer_type, int32_t peer_info,
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

void ServerManager_Center::ProcessGateServerAdded(int32_t peer_info)
{
	PktServerSync pktAdd;
	pktAdd.set_type(PktServerSync_SyncType_add);
	AddServerInfo2Pkt(PeerType_t::centerserver, pktAdd);
	AddServerInfo2Pkt(PeerType_t::worldserver, pktAdd);
	AddServerInfo2Pkt(PeerType_t::gameserver, pktAdd);
	m_pPktProcessor->SendPacket(PeerInfo::GetChannelIndex(peer_info), peer_info, 0, pktAdd);

	BroadCastServerInfoChanged(PeerType_t::gateserver, peer_info, PktServerSync_SyncType_add);
}
void ServerManager_Center::ProcessCenterServerAdded(int32_t peer_info)
{
	// log
}
void ServerManager_Center::ProcessWorldServerAdded(int32_t peer_info)
{
	PktServerSync pktAdd;
	pktAdd.set_type(PktServerSync_SyncType_add);
	AddServerInfo2Pkt(PeerType_t::centerserver, pktAdd);
	AddServerInfo2Pkt(PeerType_t::gateserver, pktAdd);
	AddServerInfo2Pkt(PeerType_t::gameserver, pktAdd);
	m_pPktProcessor->SendPacket(PeerInfo::GetChannelIndex(peer_info), peer_info, 0, pktAdd);

	BroadCastServerInfoChanged(PeerType_t::worldserver, peer_info, PktServerSync_SyncType_add);
}
void ServerManager_Center::ProcessGameServerAdded(int32_t peer_info)
{
	PktServerSync pktAdd;
	pktAdd.set_type(PktServerSync_SyncType_add);
	AddServerInfo2Pkt(PeerType_t::centerserver, pktAdd);
	AddServerInfo2Pkt(PeerType_t::gateserver, pktAdd);
	AddServerInfo2Pkt(PeerType_t::worldserver, pktAdd);
	m_pPktProcessor->SendPacket(PeerInfo::GetChannelIndex(peer_info), peer_info, 0, pktAdd);

	BroadCastServerInfoChanged(PeerType_t::gameserver, peer_info, PktServerSync_SyncType_add);
}

void ServerManager_Center::ProcessGateServerRemoved(int32_t peer_info)
{
	BroadCastServerInfoChanged(PeerType_t::gateserver, peer_info, PktServerSync_SyncType_remove);
}
void ServerManager_Center::ProcessCenterServerRemoved(int32_t peer_info)
{
	// LOG
}
void ServerManager_Center::ProcessWorldServerRemoved(int32_t peer_info)
{
	BroadCastServerInfoChanged(PeerType_t::worldserver, peer_info, PktServerSync_SyncType_remove);
}
void ServerManager_Center::ProcessGameServerRemoved(int32_t peer_info)
{
	BroadCastServerInfoChanged(PeerType_t::gameserver, peer_info, PktServerSync_SyncType_remove);
}