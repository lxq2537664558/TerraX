#include "TestServer.h"

using namespace TerraX;
using namespace tutorial;


TestServer::TestServer(EventLoop* loop, int port) : RpcServer(loop, port)
{//you can use marco to wrapper it if you want;
	m_PacketDispatcher.RegPacketHandler<Person>(new PacketFunctor<Person>(std::bind(&TestServer::onMessage_Person, this, std::placeholders::_1)));
}
