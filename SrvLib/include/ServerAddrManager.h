#pragma once
#include "ComDef.h"
#include <NetDefine.h>
#include <set>

namespace TerraX
{
	class ServerAddrManager
	{
		NOCOPY(ServerAddrManager);
	public:
		ServerAddrManager() = default;
		~ServerAddrManager() = default;
		void AddServer(int32_t peer_info);
		const std::set<int32_t>& GetServerAddrs() const { return m_ServerAddrs; }
	private:
		std::set<int32_t> m_ServerAddrs;
	};
}