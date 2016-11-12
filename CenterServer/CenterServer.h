#pragma once
#include "NetServer.h"
#include "NetDefine.h"
#include "ComDef.h"

#include "ConnectionManager.h"

namespace TerraX
{
	class CenterServer : public NetServer
	{
	public:
		using NetServer::NetServer;
		
		template<class T>
		void SendPacket(NetChannel& channel, T& packet)
		{
			channel.SendMessage(1, packet);
		}

	protected:
		void OnDisconnect(NetChannel* pChannel) override final;

	private:
		PeerType_t m_ePeerType{ PeerType_t::centerserver };

		ConnectionManager m_ConnManager{ *this };
	};
}