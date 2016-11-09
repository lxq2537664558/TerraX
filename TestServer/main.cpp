#include <iostream>
#include <stdio.h>
#include "ComDef.h"
#include "EventLoop.h"
#include "TestServer.h"
#include <thread>

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	TerraX::EventLoop loop;
	TerraX::TestServer server(&loop, 9995);
#ifdef __GNUC__
	server.setThreadNum(std::thread::hardware_concurrency() * 2);
#endif
	loop.loop();
	getchar();
	return 0;
}