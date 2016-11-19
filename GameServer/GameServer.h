#pragma once
#include "ComDef.h"
#include "NetChannel.h"
#include "proto/server_packet.pb.h"
#include "NetDefine.h"
#include "EventLoop.h"

using namespace ServerPacket;
namespace TerraX
{
	class GameServer
	{
		NOCOPY(GameServer);
		MAKEINSTANCE(GameServer);
	public:
		GameServer();
		~GameServer() = default;

		bool Init(/*Config Info*/);
		void Run();
		void Exit() { m_bExit = true; }

		explicit GameServer(EventLoop* loop, const std::string& host, int port);
		template<class T>
		void SendPacket(T& packet) {
			m_pNetChannel->SendMsg(0, packet);
		}
		void OnMessage_RegisterServerRet(NetChannel& channel, PktRegisterServer& pkt);

	private:
		void ConnectFailed();
		void Connected();
		void Disconnected();

	private:
		void RegisterServer();
		int32_t GetServerInfo() { return m_PeerInfo.serialize(); }

	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		std::unique_ptr<NetChannel> m_pNetChannel;
		PeerInfo m_PeerInfo{ PeerType_t::gameserver };
	};
}