#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include <set>
#include <map>
namespace S2SPacket
{
	class PktServerSync;
	enum PktServerSync_SyncType;
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
		void InitPacketProcessor(PacketProcessor*  pPktProcessor);

		void OnServerAdded(int32_t peer_info);
		void OnServerRemoved(int32_t peer_info);
	private:
		void RegPacketHanderFunction();

		void ProcessGateServerAdded(int32_t peer_info);
		void ProcessCenterServerAdded(int32_t peer_info);
		void ProcessWorldServerAdded(int32_t peer_info);
		void ProcessGameServerAdded(int32_t peer_info);


		void ProcessGateServerRemoved(int32_t peer_info);
		void ProcessCenterServerRemoved(int32_t peer_info);
		void ProcessWorldServerRemoved(int32_t peer_info);
		void ProcessGameServerRemoved(int32_t peer_info);

		void AddServerInfo2Pkt(PeerType_t peer_type, S2SPacket::PktServerSync& pkt);

		void BroadCastServerInfoChanged(PeerType_t peer_type, int32_t peer_info, S2SPacket::PktServerSync_SyncType eSyncType);
		void BroadCastGateServerChanged(S2SPacket::PktServerSync& pkt);
		void BroadCastWorldServerChanged(S2SPacket::PktServerSync& pkt);
		void BroadCastGameServerChanged(S2SPacket::PktServerSync& pkt);


		void OnMessage_ServerSync(S2SPacket::PktServerSync* pkt);
	private:
		PacketProcessor* m_pPktProcessor{ nullptr };
		std::map<PeerType_t,std::set<int32_t>> m_ServerInfos;
	};
}