#include "CenterServer.h"
#include "NetServer.h"
#include "EventLoop.h"
#include <chrono>
#include <thread>
#include "PacketProcessor_Center.h"
using namespace TerraX;

bool CenterServer::Init()
{
	ConnectionManager::GetInstance();
	ServerManager::GetInstance().InitPacketProcessor(&PacketProcessor_Center::GetInstance());
	PeerInfo pi(PeerType_t::centerserver);
	ServerManager::GetInstance().OnServerAdded(pi.serialize());

	PacketProcessor_Center::GetInstance().Accept(9995, MAX_CONNECTION);
	return true;
}

void CenterServer::Run()
{
	while(!m_bExit)
	{

		auto start = std::chrono::steady_clock::now();
		
		PacketProcessor_Center::GetInstance().Tick();

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
