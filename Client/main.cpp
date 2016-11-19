#include <iostream>
#include <stdio.h>
#include "EventLoop.h"
#include "Client.h"

using namespace ServerPacket;
using namespace google::protobuf;
using namespace TerraX;

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	//EventLoop loop;
	//Client client(&loop, "127.0.0.1", 9995);
	//loop.loop();
	google::protobuf::ShutdownProtobufLibrary();

	getchar();
	return 0;
}