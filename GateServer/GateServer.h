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

		void OnConnector_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);
		void OnAcceptor_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);

		//Acceptor* GetAcceptor() { return m_pAcceptor.get(); }
		//Connector* GetConnector(){ return m_pConnector.get(); }
	public:
		void Register(int32_t peer_info);
		void OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt);
	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		std::unique_ptr<Acceptor> m_pAcceptor; //front-end
		std::unique_ptr<Connector> m_pConnector; //back-end
	};
}