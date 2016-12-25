#include "ServerManager.h"

using namespace TerraX;


void ServerManager::AddServer(int32_t peer_info)
{
	m_ServerAddrs.insert(peer_info);
}