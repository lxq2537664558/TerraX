#pragma once

#include "RpcChannel.h"
#include "addressbook.pb.h"
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
		void onMessage_Person(tutorial::Person& p) 
		{
			std::cout << p.name().c_str() << std::endl;
		}
	private:
		PacketDispatcher m_PktDisPatcher;
		RpcChannel m_Channel;
	};
}