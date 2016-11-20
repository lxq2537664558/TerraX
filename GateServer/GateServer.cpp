#include "GateServer.h"
#include "NetServer.h"
#include "EventLoop.h"
#include <chrono>
#include <thread>
using namespace TerraX;

GateServer::GateServer()
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterServer>(new PacketFunctor<PktRegisterServer>(
		std::bind(&GateServer::OnMessage_RegisterServerRet, this, std::placeholders::_1, std::placeholders::_2)));
}

bool GateServer::Init()
{
	m_pAcceptor.reset(new Acceptor<GateServer>(&m_loop, 9991));
	m_pConnector.reset(new Connector<GateServer, PeerType_t::gateserver>(&m_loop, "127.0.0.1", 9995));

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


void GateServer::RegisterServer(PeerInfo& peerinfo)
{
	PktRegisterServer pkt;
	pkt.set_server_info(peerinfo.serialize());
	m_pConnector->SendPacket(pkt);
}

void GateServer::OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt)
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	std::cout << "Server: " << int32_t(pi.peer_type) << "\tIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << int32_t(pi.client_index) << std::endl;
	if (pi.peer_index <= 0) {
		std::cout << "Register failed!" << std::endl;
		channel.ForceClose();
	}
}