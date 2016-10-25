#include <iostream>
#include "EventLoop.h"
#include "RpcServer.h"


int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	TerraX::EventLoop loop;
	TerraX::RpcChannel channel(&loop, "127.0.0.1", 9995);
	std::string msg("hello, world");
	channel.SendMsg(msg);
	loop.loop();
	getchar();
	return 0;
}