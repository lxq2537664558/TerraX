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
	m_pConnector.reset(new Connector(PeerType_t::gateserver, &m_loop, "127.0.0.1", 9995));
	m_pConnector->SetNetEventCB(std::bind(&GateServer::OnConnector_NetEvent, this, std::placeholders::_1, std::placeholders::_2));

	m_pAcceptor.reset(new Acceptor(&m_loop, 9991, MAX_CONNECTION));
	m_pAcceptor->SetNetEventCB(std::bind(&GateServer::OnAcceptor_NetEvent, this, std::placeholders::_1, std::placeholders::_2));


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
	assert(pi.peer_type == uint8_t(PeerType_t::gateserver));
	assert(pi.peer_index > 0);
	assert(pi.channel_index != 0 && channel->GetChannelIndex() == 0);
	std::cout << "Server: " << pi.server_name() << "\t PeerIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << pi.channel_index << std::endl;
	channel->SetPeerInfo(server_info);
}

void GateServer::OnConnector_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if(eEvent == NetEvent_t::eConnected) {
		Register(channel->GetPeerInfo());
	} 
	else if (eEvent == NetEvent_t::eConnectFailed) {
		// do exit...
	}
	else if (eEvent == NetEvent_t::eDisconnected) {
		// do disconnect...
	}
	else {
		// unknown event
	}
}

void GateServer::OnAcceptor_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent)
{
	if (eEvent == NetEvent_t::eConnected) {

	}
	else if (eEvent == NetEvent_t::eConnectFailed) {

	}
	else if (eEvent == NetEvent_t::eDisconnected) {

	}
	else {

	}
}