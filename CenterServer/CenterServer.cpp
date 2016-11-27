#include "CenterServer.h"
#include "NetServer.h"
#include "EventLoop.h"
#include <chrono>
#include <thread>
using namespace TerraX;

bool CenterServer::Init()
{
	m_pAcceptor.reset(new Acceptor(&m_loop, 9995, MAX_CONNECTION));
	m_pAcceptor->SetNetEventCB(std::bind(&CenterServer::OnAcceptor_NetEvent, this, std::placeholders::_1, std::placeholders::_2));
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

void CenterServer::OnAcceptor_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if (eEvent == NetEvent_t::eConnected) {
	}
	else if (eEvent == NetEvent_t::eConnectFailed) {

	}
	else if (eEvent == NetEvent_t::eDisconnected) {
		m_ConnManager.OnChannel_DisConnect(channel);
	}
	else {

	}
}