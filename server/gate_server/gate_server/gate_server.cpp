#include "gate_server.h"
#include <chrono>
#include <thread>
#include "gate_net_module.h"
using namespace terra;

bool GateServer::Init()
{
	GateNetModule::GetInstance().Init();
	return true;
}

bool GateServer::AfterInit()
{
	GateNetModule::GetInstance().AfterInit();
	return true;
}

bool GateServer::Execute()
{
	while (!exit_) {
		GateNetModule::GetInstance().Execute();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return true;
}

bool GateServer::BeforeShut()
{
	return true;
}

bool GateServer::Shut()
{
	return true;
}

