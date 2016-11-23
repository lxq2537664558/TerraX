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
		void SendPacket(NetChannel& channel, Packet& packet);
		//template<class Packet>
		//void SendPacket(int32_t peer_info, Packet& packet);

		void ForceClose(NetChannel& channel);
	
	private:
		void OnDisconnect(NetChannel& channel);
	private:
		NetServer m_Acceptor;
	};

	template<class T, uint16_t max_connections>
	Acceptor<T, max_connections>::Acceptor(EventLoop* pLoop, int port)
		: m_Acceptor(pLoop, port, max_connections) {
		m_Acceptor.RegDisconnected_Callback(std::bind(&Acceptor<T, max_connections>::OnDisconnect, this, std::placeholders::_1));
	}

	template<class T, uint16_t max_connections>
	void Acceptor<T, max_connections>::OnDisconnect(NetChannel& channel) {
		//T::GetInstance().OnAcceptor_Disconnect(channel);
	}

	template<class T, uint16_t max_connections>
	void Acceptor<T, max_connections>::ForceClose(NetChannel& channel) {
		m_Acceptor.ForceClose(channel);
	}

	template<class T, uint16_t max_connections>
	template<class Packet>
	void Acceptor<T, max_connections>::SendPacket(NetChannel& channel, Packet& packet) {
		channel.SendMsg(1, packet);
	}

	//template<class T, uint16_t max_connections>
	//template<class Packet>
	//void Acceptor<T, max_connections>::SendPacket(int32_t peer_info, Packet& packet) {
		//channel.SendMsg(1, packet);
	//}
}