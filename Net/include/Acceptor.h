#pragma once

#include <memory>
#include "NetDefine.h"
#include <functional>
#include "NetServer.h"
namespace TerraX
{
	template<class T>
	class Acceptor
	{
	public:
		Acceptor(EventLoop* pLoop, int port);
		~Acceptor() = default;

		template<class Packet>
		void SendPacket(NetChannel& channel, Packet& packet);
		void ForceClose(NetChannel& channel);
	
	private:
		void OnDisconnect(NetChannel* pChannel);
	private:
		NetServer m_Acceptor;
	};

	template<class T>
	Acceptor<T>::Acceptor(EventLoop* pLoop, int port)
		: m_Acceptor(pLoop, port) {
		m_Acceptor.RegDisconnected_Callback(std::bind(&Acceptor<T>::OnDisconnect, this, std::placeholders::_1));
	}

	template<class T>
	void Acceptor<T>::OnDisconnect(NetChannel* pChannel) {
		T::GetInstance().OnAcceptor_ChannelDisconnect(pChannel);
	}

	template<class T>
	void Acceptor<T>::ForceClose(NetChannel& channel) {
		m_Acceptor.ForceClose(channel);
	}

	template<class T>
	template<class Packet>
	void Acceptor<T>::SendPacket(NetChannel& channel, Packet& packet) {
		channel.SendMsg(1, packet);
	}
}