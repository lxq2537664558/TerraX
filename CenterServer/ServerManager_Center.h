#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include "ServerManager.h"
#include "proto/server_server.pb.h"

namespace TerraX
{
	class PacketProcessor_Center;
	class ServerManager_Center : public ServerManager
	{
		DISABLE_COPY(ServerManager_Center);
		MAKE_INSTANCE(ServerManager_Center);

	public:
		ServerManager_Center() = default;
		~ServerManager_Center() = default;

		void InitPacketProcessor(PacketProcessor_Center* pPktProcessor);

		void OnServerAdded(int32_t peer_info);
		void OnServerRemoved(int32_t peer_info);

	protected:
		void ProcessGateServerAdded(int32_t peer_info);
		void ProcessCenterServerAdded(int32_t peer_info);
		void ProcessWorldServerAdded(int32_t peer_info);
		void ProcessGameServerAdded(int32_t peer_info);

		void ProcessGateServerRemoved(int32_t peer_info);
		void ProcessCenterServerRemoved(int32_t peer_info);
		void ProcessWorldServerRemoved(int32_t peer_info);
		void ProcessGameServerRemoved(int32_t peer_info);

		void AddServerInfo2Pkt(PeerType_t peer_type, S2SPacket::PktServerSync& pkt);

		void BroadCastServerInfoChanged(PeerType_t peer_type, int32_t peer_info,
			S2SPacket::PktServerSync_SyncType eSyncType);
		void BroadCastGateServerChanged(S2SPacket::PktServerSync& pkt);
		void BroadCastWorldServerChanged(S2SPacket::PktServerSync& pkt);
		void BroadCastGameServerChanged(S2SPacket::PktServerSync& pkt);
		void SendPacket2Server(S2SPacket::PktServerSync& pkt, PeerType_t ePeerType);

	private:
		PacketProcessor_Center* m_pPktProcessor{ nullptr };
	};
}