#include "Client.h"
#include "GameStateManager.h"
#include "NetManagerClient.h"
using namespace TerraX;

Client::Client()
{
}


bool Client::Init()
{
	return true;
}

void Client::Run()
{
	while (!m_bExit)
	{
		auto start = std::chrono::steady_clock::now();

		RunGame();

		ProcessPacket();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
		auto end = std::chrono::steady_clock::now();
		auto costms = std::chrono::duration_cast<std::chrono::milliseconds>
			(end - start).count();
		if (costms < 50) {
			std::this_thread::sleep_for(std::chrono::milliseconds(50 - costms));
		}
	}
}

void Client::RunGame()
{
	GameStateManager::GetInstance().Tick();

}

void Client::ProcessPacket()
{
	//Process Packet
	NetManagerClient::GetInstance().Tick();
}