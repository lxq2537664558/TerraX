#pragma once
#include "ComDef.h"
#include "NetChannel.h"
#include "proto/server_server.pb.h"
#include "NetDefine.h"
#include "EventLoop.h"

using namespace S2SPacket;
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

		void Register(int32_t peer_info);
		void OnMessage_RegisterResult(NetChannelPtr& channel, PktRegisterServer& pkt);


		void OnBackEnd_NetEvent(NetChannelPtr& channel, NetEvent_t eEvent);

	private:
		bool m_bExit{ false };
		EventLoop m_loop;

		std::unique_ptr<BackEnd> m_pBackEnd;
	};
}