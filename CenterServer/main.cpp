#include <iostream>
#include <stdio.h>
#include "CenterServer.h"
#include <thread>

using namespace S2SPacket;
using namespace google::protobuf;
using namespace TerraX;

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	if (CenterServer::GetInstance().Init())
	{
		CenterServer::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();
	getchar();
	return 0;
}