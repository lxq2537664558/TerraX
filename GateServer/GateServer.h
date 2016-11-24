#pragma once
#include "NetDefine.h"
#include "ComDef.h"
#include "EventLoop.h"
#include "proto/server_server.pb.h"
#include "proto/client_server.pb.h"
#include "Acceptor.h"
#include "Connector.h"
using namespace S2SPacket;

namespace TerraX
{
	const int MAX_CONNECTION = 1024;
	class GateServer final
	{
		NOCOPY(GateServer);
		MAKEINSTANCE(GateServer);
	public:
		GateServer();
		~GateServer() = default;

		bool Init(/*Config Info*/);
		void Run();
		void Exit() { m_bExit = true; }

		void OnAcceptor_Disconnect(int32_t peer_info);

		//Acceptor<GateServer, MAX_CONNECTION>* GetAcceptor() { return m_pAcceptor.get(); }
		//Connector<GateServer, PeerType_t::gateserver>* GetConnector(){ return m_pConnector.get(); }
	public:
		void Register(int32_t peer_info);
		void OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt);
	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		std::unique_ptr<Acceptor<GateServer, MAX_CONNECTION> > m_pAcceptor; //front-end
		std::unique_ptr<Connector<GateServer, PeerType_t::gateserver> > m_pConnector; //back-end
	};
}