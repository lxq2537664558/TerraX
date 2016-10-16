#include <iostream>
#include "EventLoop.h"
#include "RpcServer.h"

int main(int argc, char* argv[])
{
	TerraX::EventLoop loop;
	TerraX::RpcServer server(&loop, 12345);

	loop.loop();
	getchar();
	return 0;
	std::cout << "hello world!" << std::endl;
	getchar();
	return 0;
}