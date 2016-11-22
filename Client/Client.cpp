#include "Client.h"

using namespace TerraX;

Client::Client()
{
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
