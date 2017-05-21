#pragma once

#include "base/types.h"
#include "base/module_interface.h"
namespace terra
{
	class GateServer final : public IModule
	{
		DISABLE_COPY(GateServer);
		MAKE_INSTANCE(GateServer);

	public:
		GateServer() = default;
		~GateServer() = default;

		bool Init() override;
		bool AfterInit() override;
		bool Execute() override;
		bool BeforeShut() override;
		bool Shut() override;

	private:
		bool exit_{ false };
		//std::vector<class IMoudle*> modules_;
	};
}