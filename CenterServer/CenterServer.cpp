#include "CenterServer.h"
#include "NetServer.h"
using namespace TerraX;

void CenterServer::OnDisconnect(NetChannel* pChannel)
{
	m_ConnManager.UnregisterServer(pChannel);
	NetServer::OnDisconnect(pChannel);
}
