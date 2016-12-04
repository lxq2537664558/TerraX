#pragma once
#include "ComDef.h"
#include "NetChannel.h"
#include "proto/server_server.pb.h"
#include "NetDefine.h"
#include "EventLoop.h"

using namespace S2SPacket;
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

	private:
		bool m_bExit{ false };
		EventLoop m_loop;
		
	};
}