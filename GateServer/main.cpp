#include <iostream>
#include <stdio.h>
#include "GateServer.h"

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

	if (GateServer::GetInstance().Init())
	{
		GateServer::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();

	getchar();
}