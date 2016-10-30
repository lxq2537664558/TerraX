#include <iostream>
#include "EventLoop.h"
#include "RpcServer.h"

#include "TestClient.h"
using namespace tutorial;
using namespace google::protobuf;
using namespace TerraX;

int main(int argc, char* argv[])
{
	GOOGLE_PROTOBUF_VERIFY_VERSION;
#ifdef _WIN32
	WSADATA wsa_data;
	WSAStartup(0x0201, &wsa_data);
#endif
	EventLoop loop;
	TestClient client(&loop, "127.0.0.1", 9995);

	Person p;
	p.set_name("trump");
	p.set_id(12345);
	p.set_email("hello@world.com");
	client.sendPacket(p);
	loop.loop();
	google::protobuf::ShutdownProtobufLibrary();

	getchar();
	return 0;
}