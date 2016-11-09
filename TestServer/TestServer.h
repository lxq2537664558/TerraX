#pragma once
#include "RpcServer.h"
#include "proto/addressbook.pb.h"
#include "ComDef.h"
#include <thread>

namespace TerraX
{
	class TestServer : public RpcServer
	{
		NOCOPY(TestServer);
	public:
		explicit TestServer(EventLoop* loop, int port);
		template<class T>
		void sendPacket(RpcChannel& channel, T& packet)
		{
			channel.sendMessage(1, packet);
		}
		void onMessage_Person(RpcChannel& channel, tutorial::Person& p)
		{
			//auto now = std::chrono::high_resolution_clock::now();
			//std::cout << GetTickCount64() << std::endl;
			std::cout << "Thread-" << std::this_thread::get_id() << ": " << p.name().c_str() << std::endl;
			sendPacket(channel, p);
		}
	};
}
