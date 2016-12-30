#include <iostream>
#include <stdio.h>
#include <google/protobuf/message.h>
#include "EventLoop.h"
#include "WorldServer.h"

using namespace google::protobuf;
using namespace TerraX;

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	
	if (WorldServer::GetInstance().Init())
	{
		WorldServer::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();

	getchar();
	return 0;
}