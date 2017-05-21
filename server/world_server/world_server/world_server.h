#pragma once

#include "base/types.h"
#include "base/module_interface.h"

namespace terra
{
	class WorldServer final : public IModule
	{
		DISABLE_COPY(WorldServer);
		MAKE_INSTANCE(WorldServer);

	public:
		WorldServer() = default;
		~WorldServer() = default;

		bool Init() override;
		bool AfterInit() override;
		bool Execute() override;
		bool BeforeShut() override;
		bool Shut() override;

	private:
		bool exit_{ false };
	};
}