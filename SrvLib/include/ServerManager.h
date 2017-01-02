#pragma once
#include <map>
#include <set>
#include "ComDef.h"
#include "NetDefine.h"
namespace S2SPacket
{
	class PktServerSync;
}
namespace TerraX
{
	class PacketProcessor;
	class ServerManager
	{
		NOCOPY(ServerManager);
		MAKEINSTANCE(ServerManager);

	public:
		ServerManager();
		~ServerManager() = default;

		int GetCenterPeerInfo();
		int GetWorldPeerInfo();

	private:
		void RegPacketHanderFunction();

		void OnMessage_PktServerSync(S2SPacket::PktServerSync* pkt);

	protected:
		std::map<PeerType_t, std::set<int32_t>> m_ServerInfos;
	};
}