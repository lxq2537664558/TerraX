#pragma once

#include "base/types.h"
#include "base/module_interface.h"
#include "node_net_module.h"
//#include <vector>
namespace terra
{
	class NodeServer final : public IModule
	{
		DISABLE_COPY(NodeServer);
		MAKE_INSTANCE(NodeServer);

	public:
		NodeServer() = default;
		~NodeServer() = default;

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