#pragma once
#include "NetDefine.h"
#include "ComDef.h"
#include "EventLoop.h"
#include "proto/server_packet.pb.h"
#include "Acceptor.h"
#include "Connector.h"
#include "CltConnectionManager.h"
using namespace ServerPacket;

namespace TerraX
{
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

		void OnAcceptor_ChannelDisconnect(NetChannel* pChannel) {}

		Acceptor<GateServer>* GetAcceptor() { return m_pAcceptor.get(); }
		Connector<GateServer, PeerType_t::gateserver>* GetConnector(){ return m_pConnector.get(); }
	public:
		//void ForceClose(NetChannel& channel);
		void RegisterServer(PeerInfo& peerinfo); 
		void OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt);
	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		CltConnectionManager m_CltConnManager{ *this };
		std::unique_ptr<Acceptor<GateServer> > m_pAcceptor; //front-end
		std::unique_ptr<Connector<GateServer, PeerType_t::gateserver> > m_pConnector; //back-end
	};
}