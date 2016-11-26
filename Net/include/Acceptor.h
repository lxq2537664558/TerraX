#pragma once

#include <memory>
#include "NetDefine.h"
#include <functional>
#include "NetServer.h"
namespace TerraX
{
	template<class T, uint16_t max_connections>
	class Acceptor
	{
	public:
		Acceptor(EventLoop* pLoop, int port);
		~Acceptor() = default;

		template<class Packet>
		void SendPacket(NetChannelPtr& channel, Packet& packet);
		//template<class Packet>
		//void SendPacket(int32_t peer_info, Packet& packet);

		void ForceClose(NetChannelPtr& channel);
		
		void SetNetEventCB(NetEvent_CB cb) { m_Acceptor.RegNetEvent_Callback(cb); }
	private:
		NetServer m_Acceptor;
	};

	template<class T, uint16_t max_connections>
	Acceptor<T, max_connections>::Acceptor(EventLoop* pLoop, int port)
		: m_Acceptor(pLoop, port, max_connections) {
	}

	template<class T, uint16_t max_connections>
	void Acceptor<T, max_connections>::ForceClose(NetChannelPtr& channel) {
		m_Acceptor.ForceClose(channel);
	}

	template<class T, uint16_t max_connections>
	template<class Packet>
	void Acceptor<T, max_connections>::SendPacket(NetChannelPtr& channel, Packet& packet) {
		channel->SendMsg(1, packet);
	}

	//template<class T, uint16_t max_connections>
	//template<class Packet>
	//void Acceptor<T, max_connections>::SendPacket(int32_t peer_info, Packet& packet) {
		//channel->SendMsg(1, packet);
	//}
}