#include "WorldServer.h"
#include "NetManagerWorld.h"
using namespace TerraX;

WorldServer::WorldServer()
{

}


bool WorldServer::Init()
{
	NetManagerWorld::GetInstance().Connect("127.0.0.1", 9995);
	return true;
}

void WorldServer::Run()
{
	while (!m_bExit)
	{

		auto start = std::chrono::steady_clock::now();

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