#pragma once

#include "RpcChannel.h"
#include "proto/addressbook.pb.h"
#include "PacketDispatcher.h"

namespace TerraX
{
	class TestClient
	{
		NOCOPY(TestClient);
	public:
		explicit TestClient(EventLoop* loop, const std::string& host, int port);
		template<class T>
		void sendPacket(T& packet) 
		{
			m_Channel.sendMessage(0, packet);
		}
		void onMessage_Person(RpcChannel& channel, tutorial::Person& p) 
		{
			//auto now = std::chrono::high_resolution_clock::now();
			//std::cout << GetTickCount64() << std::endl;
			std::cout << p.name().c_str() << std::endl;
		}
	private:
		RpcChannel m_Channel;
	};
}