#include "CenterServer.h"
#include "NetServer.h"
#include "EventLoop.h"
using namespace TerraX;

bool CenterServer::Init()
{
	m_pNetFrontend.reset(new NetServer(&m_loop, 9995));
	m_pNetFrontend->RegDisconnected_Callback(std::bind(&CenterServer::OnDisconnect, this, std::placeholders::_1));

#ifdef __GNUC__
	// we don't need multithread
	//m_pNetServer->SetThreadNum(std::thread::hardware_concurrency() * 2);
#endif
	return true;
}

void CenterServer::Run()
{
	while(!m_bExit)
	{
		m_loop.loop();
	}
}

void CenterServer::ForceClose(NetChannel& channel)
{
	assert(m_pNetFrontend); m_pNetFrontend->ForceClose(channel);
}

void CenterServer::OnDisconnect(NetChannel* pChannel)
{
	m_ConnManager.UnregisterServer(pChannel);
}
