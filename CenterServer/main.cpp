#include <google/protobuf/message.h>
#include <stdio.h>
#include <iostream>
#include "CenterServer.h"
#ifdef _WIN32
#include <WinSock2.h>
#endif

using namespace google::protobuf;
using namespace TerraX;

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	GOOGLE_PROTOBUF_VERIFY_VERSION;

	if (CenterServer::GetInstance().Init()) {
		CenterServer::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();
#ifdef _WIN32
	WSACleanup();
#endif
	getchar();
	return 0;
}