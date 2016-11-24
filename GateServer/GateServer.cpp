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
		std::bind(&GateServer::OnMessage_RegisterResult, this, std::placeholders::_1, std::placeholders::_2)));
}

bool GateServer::Init()
{
	m_pAcceptor.reset(new Acceptor<GateServer, MAX_CONNECTION>(&m_loop, 9991));
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


void GateServer::Register(int32_t peer_info)
{
	PktRegisterServer pkt;
	pkt.set_server_info(peer_info);
	m_pConnector->SendPacket(pkt);
}

void GateServer::OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt)
{
	int32_t server_info = pkt.server_info();
	PeerInfo pi;
	pi.parse(server_info);
	assert(pi.channel_index == 0 && channel->GetChannelIndex() != 0);
	std::cout << "Server: " << pi.server_name() << 
		"\t ChannelIndex: " << int32_t(channel->GetChannelIndex()) << std::endl;
	channel->SetPeerType(pi.peer_type);
}

void GateServer::OnAcceptor_Disconnect(int32_t peer_info)
{
	
}