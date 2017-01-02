#include <chrono>
#include <thread>
#include "CenterServer.h"
#include "ConnectionManager.h"
#include "PacketProcessor_Center.h"
#include "ServerManager_Center.h"
#include "LoginManager.h"
using namespace TerraX;

bool CenterServer::Init()
{
    if (!InitStaticModule()) {
        return false;
    }
    if (!InitNetModule()) {
        return false;
    }
    return true;
}

bool CenterServer::InitStaticModule()
{
	PacketProcessor_Center::GetInstance();
    ConnectionManager::GetInstance();
	LoginManager::GetInstance();
    ServerManager_Center::GetInstance().InitPacketProcessor(&PacketProcessor_Center::GetInstance());
    return true;
}
bool CenterServer::InitNetModule()
{
    PeerInfo pi(PeerType_t::centerserver);
	ServerManager_Center::GetInstance().OnServerAdded(pi.serialize());

    PacketProcessor_Center::GetInstance().Accept(9995, MAX_CONNECTION);
    return true;
}
void CenterServer::ProcessLogic() { PacketProcessor_Center::GetInstance().Tick(); }

void CenterServer::Run()
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
