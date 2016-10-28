#pragma once
#include "RpcServer.h"
#include "addressbook.pb.h"

namespace TerraX
{
	class TestServer : public RpcServer
	{
	public:
		explicit TestServer(EventLoop* loop, int port);
		template<class T>
		void sendPacket(T& packet)
		{
			//std::string msg;
			//packet.SerializeToString(&msg);
			//m_Channel.SendMessage(0, T::descriptor()->name(), msg);
		}
		void onMessage_Person(tutorial::Person& p)
		{
			std::cout << p.name().c_str() << std::endl;
			//sendPacket(p);
		}
	};
}
