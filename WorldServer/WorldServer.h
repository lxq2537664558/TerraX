#pragma once
#include "ComDef.h"
#include "IServer.h"

namespace TerraX
{
	class WorldServer final : public IServer
	{
		NOCOPY(WorldServer);
		MAKEINSTANCE(WorldServer);
	public:
		WorldServer();
		~WorldServer() = default;

		bool Init(/*Config Info*/) override;
		void Run() override;
		void Exit() override { m_bExit = true; }

	protected:
		bool InitStaticModule() override;
		bool InitNetModule() override;
		void ProcessLogic() override;

	private:
		bool m_bExit{ false };
	};
}