#include <iostream>
#include "ComDef.h"
#include "EventLoop.h"
#include "TestServer.h"

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	TerraX::EventLoop loop;
	TerraX::TestServer server(&loop, 9995);
	loop.loop();
	getchar();
	return 0;
}