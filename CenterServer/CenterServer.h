#pragma once
#include "NetServer.h"
#include "NetDefine.h"
#include "ComDef.h"
#include "ConnectionManager.h"
#include "EventLoop.h"

namespace TerraX
{
	class CenterServer final
	{
		NOCOPY(CenterServer);
		MAKEINSTANCE(CenterServer);
	public:
		CenterServer() = default;
		~CenterServer() = default;

		bool Init(/*Config Info*/);
		void Run();
		void Exit() { m_bExit = true; }

		template<class T>
		void SendPacket(NetChannel& channel, T& packet)
		{
			channel.SendMsg(1, packet);
		}
	public:
		void ForceClose(NetChannel& channel); 
	private:
		void OnDisconnect(NetChannel* pChannel);

	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		std::unique_ptr<NetServer> m_pNetFrontend; //fore-end , back-end
		std::unique_ptr<NetChannel> m_pNetBackend;
		PeerType_t m_ePeerType{ PeerType_t::centerserver };

		ConnectionManager m_ConnManager{ *this };
	};
}