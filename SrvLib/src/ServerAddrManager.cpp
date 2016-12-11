#include "ServerAddrManager.h"

using namespace TerraX;


void ServerAddrManager::AddServer(int32_t peer_info)
{
	m_ServerAddrs.insert(peer_info);
}