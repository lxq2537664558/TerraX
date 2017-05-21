#pragma once

#include "base/types.h"
#include "srv/net/net_base_module.h"
#include "srv/net/server_accept_service.h"

namespace terra
{
	class WorldNetModule : public NetBaseModule
	{
		DISABLE_COPY(WorldNetModule);
		MAKE_INSTANCE(WorldNetModule);
	private:
		std::unique_ptr<ServerAcceptService> accept_service_;
	public:
		WorldNetModule() = default;
		~WorldNetModule() = default;

		bool Init();
		bool AfterInit();
		bool Execute();
		bool BeforeShut();
		bool Shut();
	private:
		void InitWorldNetInfo();
	};
}