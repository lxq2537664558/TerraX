#include "TestServer.h"

using namespace TerraX;
using namespace tutorial;


TestServer::TestServer(EventLoop* loop, int port) : RpcServer(loop, port)
{//you can use marco to wrapper it if you want;
	PacketDispatcher::GetInstance().RegPacketHandler<Person>(new PacketFunctor<Person>(
		std::bind(&TestServer::onMessage_Person, this, std::placeholders::_1, std::placeholders::_2)));
}
