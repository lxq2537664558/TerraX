#pragma once
#include "NetServer.h"
#include "NetDefine.h"
#include "ComDef.h"
#include "EventLoop.h"
#include "proto/server_packet.pb.h"
using namespace ServerPacket;

namespace TerraX
{
	class GateServer final
	{
		NOCOPY(GateServer);
		MAKEINSTANCE(GateServer);
	public:
		GateServer() = default;
		~GateServer() = default;

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
		void OnAcceptorDisconnect(NetChannel* pChannel);

		void ConnectorConnectFailed();
		void ConnectorConnected();
		void ConnectorDisconnected();
		void RegisterServer();
		int32_t GetServerInfo() { return m_PeerInfo.serialize(); }
	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		std::unique_ptr<NetServer> m_pNetAcceptor; //fore-end , back-end
		std::unique_ptr<NetChannel> m_pNetConnector;
		PeerInfo m_PeerInfo{ PeerType_t::gateserver };
	};
}