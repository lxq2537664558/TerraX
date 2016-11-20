#include <iostream>
#include <stdio.h>
#include "EventLoop.h"
#include "WorldServer.h"

//using namespace S2SPacket;
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

	//EventLoop loop;
	//WorldServer gameserver(&loop, "127.0.0.1", 9995);
	//loop.loop();
	google::protobuf::ShutdownProtobufLibrary();

	getchar();
	return 0;
}