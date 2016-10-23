#include <iostream>
#include "ComDef.h"
//#include "ServerBase.h"
#include "EventLoop.h"
#include "RpcServer.h"

int main(int argc, char* argv[])
{
#ifdef WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	TerraX::EventLoop loop;
	TerraX::RpcServer server(&loop, 9995);
	loop.loop();
	getchar();
	return 0;
}