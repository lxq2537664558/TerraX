#pragma once
#include "ComDef.h"
#include "NetChannel.h"
#include "proto/client_server.pb.h"
#include "NetDefine.h"
#include "Connector.h"
#include "EventLoop.h"

using namespace C2SPacket;
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

		void Register(int32_t peer_info) {};

	private:
		bool m_bExit{ false };
		EventLoop m_loop;

		std::unique_ptr<Connector<Client, PeerType_t::client> > m_pConnector;

	};
}