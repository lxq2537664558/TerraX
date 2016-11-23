#pragma once
#include "NetServer.h"
#include "NetDefine.h"
#include "ComDef.h"
#include "ConnectionManager.h"
#include "EventLoop.h"
#include "Acceptor.h"

namespace TerraX
{
	const int MAX_CONNECTION = 64;
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

		void OnAcceptor_Disconnect(int32_t peer_info) {};
	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		ConnectionManager m_ConnManager{ *this };

		std::unique_ptr<Acceptor<CenterServer, MAX_CONNECTION> > m_pAcceptor; //front-end
		//std::unique_ptr<Connector<NetChannel, PeerType_t::gateserver> > m_pConnector; //back-end
	};
}