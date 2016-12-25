#include "GateServer.h"
#include "NetServer.h"
#include "EventLoop.h"
#include "GatePacketProcessor.h"
#include <chrono>
#include <thread>
using namespace TerraX;

GateServer::GateServer()
{
}

bool GateServer::Init()
{
	GatePacketProcessor::GetInstance().Connect("127.0.0.1", 9995);
	GatePacketProcessor::GetInstance().Accept(9991, MAX_CONNECTION);
	return true;
}

void GateServer::Run()
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


void GateServer::ProcessLogic()
{
	GatePacketProcessor::GetInstance().Tick();
}
