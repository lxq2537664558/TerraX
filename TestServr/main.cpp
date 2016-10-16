#include <iostream>
#include "ComDef.h"
//#include "ServerBase.h"
#include "EventLoop.h"
#include "RpcServer.h"
int main(int argc, char* argv[])
{
	TerraX::EventLoop loop;
	TerraX::RpcServer server(&loop, 12345);
	loop.loop();
	getchar();
	return 0;
}