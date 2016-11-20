#pragma once
#include "ComDef.h"
#include "NetChannel.h"
#include "proto/server_packet.pb.h"
#include "NetDefine.h"
#include "Connector.h"
#include "EventLoop.h"

using namespace ServerPacket;
namespace TerraX
{
	class WorldServer 
	{
		NOCOPY(WorldServer);
		MAKEINSTANCE(WorldServer);
	public:
		WorldServer();
		~WorldServer() = default;

		bool Init(/*Config Info*/);
		void Run();
		void Exit() { m_bExit = true; }

		void RegisterServer(PeerInfo& peerinfo);
		void OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt);

	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		
		std::unique_ptr<Connector<WorldServer, PeerType_t::worldserver> > m_pConnector;
	};
}