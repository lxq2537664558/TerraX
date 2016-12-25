#pragma once
#include "NetDefine.h"
#include "ComDef.h"
#include "EventLoop.h"
#include "proto/server_server.pb.h"
#include "proto/client_server.pb.h"
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

		//FrontEnd* GetFrontEnd() { return m_pFrontEnd.get(); }
		//BackEnd* GetBackEnd(){ return m_pBackEnd.get(); }
	private:
		void ProcessLogic();

	private:
		bool m_bExit{ false };
	};
}