#include <iostream>
#include "EventLoop.h"
#include "RpcServer.h"
#include "PacketDispatcher.h"
#include "addressbook.pb.h"
#include "PacketDispatcher.h"
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
	RpcChannel channel(&loop, "127.0.0.1", 9995);

	Person p;
	p.set_name("trump");
	p.set_id(12345);
	p.set_email("hello@world.com");
	std::cout << p.GetTypeName().c_str() << std::endl;
	std::cout << Person::descriptor()->name() << std::endl;
	const Descriptor* pDesc = DescriptorPool::generated_pool()->FindMessageTypeByName(p.GetTypeName());
	const Message* pProtoType = MessageFactory::generated_factory()->GetPrototype(pDesc);
	//const Message* pMsg = pProtoType->New();

	auto cb = std::bind(&RpcChannel::OnMessage, &channel, std::placeholders::_1);
	channel.m_PacketDisPatcher.RegPacketHandler<Person>(new PacketFunctor<Person>(cb));

	channel.SendPacket(p);
	loop.loop();
	google::protobuf::ShutdownProtobufLibrary();

	getchar();
	return 0;
}