#pragma once
#include "IServer.h"
#include "NetServer.h"
#include "NetDefine.h"
#include "ComDef.h"
#include "ConnectionManager.h"
#include "EventLoop.h"

namespace TerraX
{
	const int MAX_CONNECTION = 64;
	class CenterServer final : public IServer
	{
		NOCOPY(CenterServer);
		MAKEINSTANCE(CenterServer);
	public:
		CenterServer() = default;
		~CenterServer() = default;

		bool Init(/*Config Info*/) override;
		void Run() override;
		void Exit() override { m_bExit = true; }

	private:
		bool m_bExit{ false };
	};
}