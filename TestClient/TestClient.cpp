#include "TestClient.h"

using namespace TerraX;
using namespace tutorial;

TestClient::TestClient(EventLoop* loop, const std::string& host, int port) :
	m_Channel(loop, host, port)
{
	//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<Person>(new PacketFunctor<Person>(
		std::bind(&TestClient::onMessage_Person, this, std::placeholders::_1, std::placeholders::_2)));
}
