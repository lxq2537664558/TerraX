#include "Guest.h"
#include "ServerManager.h"
using namespace TerraX;


Guest::Guest(int GuestID) : m_nGuestID(GuestID), m_eGameState(GuestState_t::eWaitingAccountInfo)
{
	InitCenterConnection();
}

void Guest::InitCenterConnection()
{
	m_mapPeerInfos[PeerType_t::centerserver] = ServerManager::GetInstance().GetCenterPeerInfo();
}

int32_t Guest::GetDestPeerInfo(PeerType_t ePeerType)
{
	auto it = m_mapPeerInfos.find(ePeerType);
	return (it != m_mapPeerInfos.end()) ? it->second : 0;
}
