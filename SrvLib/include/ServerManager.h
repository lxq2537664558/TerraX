#pragma once
#include "ComDef.h"
#include <NetDefine.h>
#include <set>

namespace TerraX
{
	class ServerManager
	{
		NOCOPY(ServerManager);
	public:
		ServerManager() = default;
		~ServerManager() = default;
		void AddServer(int32_t peer_info);
		const std::set<int32_t>& GetServerAddrs() const { return m_ServerAddrs; }
	private:
		std::set<int32_t> m_ServerAddrs;
	};
}