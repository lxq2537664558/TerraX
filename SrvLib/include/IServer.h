#pragma once
#include "ComDef.h"
namespace TerraX
{
	class IServer
	{
		NOCOPY(IServer);
	protected:
		IServer() = default;
	public:
		~IServer(){}
		virtual bool Init() = 0;
		virtual void Run() = 0;
		virtual void Exit() = 0;

	protected:
		virtual bool InitStaticModule() = 0;
		virtual bool InitNetModule() = 0;


		virtual void ProcessLogic() = 0;
	};
}