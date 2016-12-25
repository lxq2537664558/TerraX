#include <stdio.h>
#include <iostream>
#include "GateServer.h"

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

	if (GateServer::GetInstance().Init()) {
		GateServer::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();

	getchar();
}