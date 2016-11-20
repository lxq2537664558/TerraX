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
	class Client
	{
		NOCOPY(Client);
		MAKEINSTANCE(Client);
	public:
		Client();
		~Client() = default;

		bool Init(/*Config Info*/);
		void Run();
		void Exit() { m_bExit = true; }

		void RegisterServer(PeerInfo& peerinfo);
		void OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt);

	private:
		bool m_bExit{ false };
		EventLoop m_loop;

		std::unique_ptr<Connector<Client, PeerType_t::client> > m_pConnector;

	};
}