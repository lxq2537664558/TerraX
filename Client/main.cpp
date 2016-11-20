#include <iostream>
#include <stdio.h>
#include "EventLoop.h"
#include "Client.h"

using namespace C2SPacket;
using namespace google::protobuf;
using namespace TerraX;

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	if (Client::GetInstance().Init())
	{
		Client::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();

	getchar();
	return 0;
}