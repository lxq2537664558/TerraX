#include "Client.h"

using namespace TerraX;

Client::Client()
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<PktRegisterClient>(new PacketFunctor<PktRegisterClient>(
		std::bind(&Client::OnMessage_RegisterRet, this, std::placeholders::_1, std::placeholders::_2)));
}


bool Client::Init()
{
	m_pConnector.reset(new Connector<Client, PeerType_t::client>(&m_loop, "127.0.0.1", 9991));

	return true;
}

void Client::Run()
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
	}
}

void Client::Register(PeerInfo& peerinfo)
{
	PktRegisterClient pkt;
	pkt.set_client_info(peerinfo.serialize());
	m_pConnector->SendPacket(pkt);
}

void Client::OnMessage_RegisterRet(NetChannel& channel, PktRegisterClient& pkt)
{
	int32_t server_info = pkt.client_info();
	PeerInfo pi;
	pi.parse(server_info);
	std::cout << "Server: " << int32_t(pi.peer_type) << "\tIndex: " << int32_t(pi.peer_index) <<
		"\t ChannelIndex: " << int32_t(pi.client_index) << std::endl;
	if (pi.client_index <= 0) {
		std::cout << "Register failed!" << std::endl;
		channel.ForceClose();
	}
}
