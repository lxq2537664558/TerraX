#include <chrono>
#include <thread>
#include "WorldServer.h"
#include "PacketProcessor_World.h"
#include "ServerManager.h"
using namespace TerraX;

WorldServer::WorldServer()
{

}

bool WorldServer::Init()
{
	if (!InitStaticModule()) {
		return false;
	}
	if (!InitNetModule()) {
		return false;
	}
	return true;
}

bool WorldServer::InitStaticModule()
{
	PacketProcessor_World::GetInstance();
	ServerManager::GetInstance();
	return true;
}
bool WorldServer::InitNetModule()
{
	PacketProcessor_World::GetInstance().Connect("127.0.0.1", 9995);
	return true;
}

void WorldServer::Run()
{
	while (!m_bExit)
	{

		auto start = std::chrono::steady_clock::now();

		ProcessLogic();
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

void WorldServer::ProcessLogic() { PacketProcessor_World::GetInstance().Tick(); }