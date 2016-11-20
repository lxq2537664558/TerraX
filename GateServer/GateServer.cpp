#include "GateServer.h"
#include "NetServer.h"
#include "EventLoop.h"
#include <chrono>
#include <thread>
using namespace TerraX;

bool GateServer::Init()
{
	m_pNetAcceptor.reset(new NetServer(&m_loop, 9991));
	m_pNetAcceptor->RegDisconnected_Callback(std::bind(&GateServer::OnDisconnect, this, std::placeholders::_1));
	

	m_pNetConnector.reset(new NetChannel(&m_loop, "127.0.0.1", 9995));
	m_pNetConnector->RegConnected_Callback(std::bind(&GateServer::ConnectorConnected, this));
	m_pNetConnector->RegConnectFailed_Callback(std::bind(&GateServer::ConnectorConnectFailed, this));
	m_pNetConnector->RegDisconnected_Callback(std::bind(&GateServer::ConnectorDisconnected, this));

	return true;
}

void GateServer::Run()
{
	while (!m_bExit)
	{

		auto start = std::chrono::steady_clock::now();

		m_loop.loop();
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

void GateServer::ForceClose(NetChannel& channel)
{
	assert(m_pNetAcceptor); m_pNetAcceptor->ForceClose(channel);
}

void GateServer::OnAcceptorDisconnect(NetChannel* pChannel)
{
	//m_ConnManager.UnregisterServer(pChannel);
}

void GateServer::ConnectorConnectFailed()
{
	//Reconnect
}

void GateServer::ConnectorConnected()
{
	RegisterServer();
}

void GateServer::ConnectorDisconnected()
{
}

void GateServer::RegisterServer()
{
	PktRegisterServer pkt;
	pkt.set_server_info(GetServerInfo());
	SendPacket(pkt);
}