#include "TestClient.h"

using namespace TerraX;
using namespace tutorial;

TestClient::TestClient(EventLoop* loop, const std::string& host, int port) :
	m_Channel(loop, host, port, m_PktDisPatcher)
{
	//you can use marco to wrapper it if you want;
	m_PktDisPatcher.RegPacketHandler<Person>(new PacketFunctor<Person>(std::bind(&TestClient::OnMessage, this, std::placeholders::_1)));
}
