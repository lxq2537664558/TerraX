#include <chrono>
#include <thread>
#include "GateServer.h"
#include "PacketProcessor_Gate.h"
#include "ServerManager.h"
#include "GateLoginManager.h"
using namespace TerraX;

GateServer::GateServer() {}

bool GateServer::InitStaticModule()
{
    PacketProcessor_Gate::GetInstance();
	ServerManager::GetInstance();
	GateLoginManager::GetInstance();
    return true;
}

bool GateServer::InitNetModule()
{
    PacketProcessor_Gate::GetInstance().Connect("127.0.0.1", 9995);
    PacketProcessor_Gate::GetInstance().Accept(9991, MAX_CONNECTION);

    return true;
}

bool GateServer::Init()
{
    InitStaticModule();
    InitNetModule();
    return true;
}

void GateServer::Run()
{
    while (!m_bExit) {
        auto start = std::chrono::steady_clock::now();

        ProcessLogic();
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        auto end = std::chrono::steady_clock::now();
        auto costms = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        if (costms < 50) {
            std::this_thread::sleep_for(std::chrono::milliseconds(50 - costms));
        }
        // std::cout << std::this_thread::get_id() << ": " << costms << std::endl;
    }
}

void GateServer::ProcessLogic() { PacketProcessor_Gate::GetInstance().Tick(); }
