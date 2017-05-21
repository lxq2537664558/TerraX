#include "node_server.h"
#include <chrono>
#include <thread>
#include "node_net_module.h"
using namespace terra;

bool NodeServer::Init()
{
	NodeNetModule::GetInstance().Init();
	return true;
}

bool NodeServer::AfterInit()
{
	NodeNetModule::GetInstance().AfterInit();
	return true;
}

bool NodeServer::Execute()
{
	while (!exit_) {
		NodeNetModule::GetInstance().Execute();
		std::this_thread::sleep_for(std::chrono::milliseconds(1));
	}
	return true;
}

bool NodeServer::BeforeShut()
{
	return true;
}

bool NodeServer::Shut()
{
	return true;
}

