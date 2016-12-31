#include <iostream>
#include <stdio.h>
#include "Client.h"
#ifdef _WIN32
#include <WinSock2.h>
#endif // _WIN32

using namespace google::protobuf;
using namespace TerraX;

int main(int argc, char* argv[])
{
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif

	GOOGLE_PROTOBUF_VERIFY_VERSION;
	if (Client::GetInstance().Init())
	{
		Client::GetInstance().Run();
	}

	google::protobuf::ShutdownProtobufLibrary();
#ifdef _WIN32
	WSACleanup();
#endif
	getchar();
	return 0;
}