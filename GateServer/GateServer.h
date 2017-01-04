#pragma once
#include "ComDef.h"
#include "IServer.h"
namespace TerraX
{
	const int MAX_CONNECTION = 1024;
	class GateServer final : public IServer
	{
		DISABLE_COPY(GateServer);
		MAKE_INSTANCE(GateServer);
	public:
		GateServer();
		~GateServer() = default;

		bool Init(/*Config Info*/) override;
		void Run() override;
		void Exit() override { m_bExit = true; }

		//FrontEnd* GetFrontEnd() { return m_pFrontEnd.get(); }
		//BackEnd* GetBackEnd(){ return m_pBackEnd.get(); }
	protected:
		bool InitStaticModule() override;
		bool InitNetModule() override;

		void ProcessLogic() override;

	private:
		bool m_bExit{ false };
	};
}