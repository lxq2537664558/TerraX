#include <chrono>
#include <thread>
#include "GameServer.h"
//#include "PacketProcessor_Game.h"
#include "ServerManager.h"
using namespace TerraX;

GameServer::GameServer()
{
}


bool GameServer::Init()
{
	if (!InitStaticModule()) {
		return false;
	}
	if (!InitNetModule()) {
		return false;
	}
	return true;
}

bool GameServer::InitStaticModule()
{
	//PacketProcessor_Game::GetInstance();
	ServerManager::GetInstance();
	return true;
}
bool GameServer::InitNetModule()
{
	//PacketProcessor_Game::GetInstance().Connect("127.0.0.1", 9995);
	return true;
}

void GameServer::Run()
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

void GameServer::ProcessLogic() { /*PacketProcessor_World::GetInstance().Tick();*/ }