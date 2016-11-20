#include "CenterServer.h"
#include "NetServer.h"
#include "EventLoop.h"
#include <chrono>
#include <thread>
using namespace TerraX;

bool CenterServer::Init()
{
	m_pNetAcceptor.reset(new NetServer(&m_loop, 9995));
	m_pNetAcceptor->RegDisconnected_Callback(std::bind(&CenterServer::OnAcceptorDisconnect, this, std::placeholders::_1));

#ifdef __GNUC__
	// we don't need multi-thread
	//m_pNetServer->SetThreadNum(std::thread::hardware_concurrency() * 2);
#endif
	return true;
}

void CenterServer::Run()
{
	while(!m_bExit)
	{

		auto start = std::chrono::steady_clock::now();
		
		m_loop.loop();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));

		auto end = std::chrono::steady_clock::now();
		auto costms = std::chrono::duration_cast<std::chrono::milliseconds>
			(end - start).count();
		if (costms < 50) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50 - costms));
		}
		//std::cout << std::this_thread::get_id() << ": " << costms << std::endl;
	}
}

void CenterServer::ForceClose(NetChannel& channel)
{
	assert(m_pNetAcceptor); m_pNetAcceptor->ForceClose(channel);
}

void CenterServer::OnAcceptorDisconnect(NetChannel* pChannel)
{
	m_ConnManager.UnregisterServer(pChannel);
}
