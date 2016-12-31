#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include <array>
#include <queue>
#include <map>
#include <cassert>
#include "ServerManager.h"

namespace S2SPacket
{
	class PktRegisterReq;
}

namespace TerraX
{
	class CenterServer;
	class NetChannel;

	const int MAX_GATE_CONNECTION_COUNT = 4;
	const int MAX_GAME_CONNECTION_COUNT = 4;
	const int MAX_WORLD_CONNECTION_COUNT = 1;

	class ConnectionManager
	{
		NOCOPY(ConnectionManager);
		MAKEINSTANCE(ConnectionManager);
	public:
		ConnectionManager(); 


		void OnChannel_DisConnect(NetChannelPtr& channel);
	private:
		uint8_t GetAvailableConnIdx(PeerType_t peer_type);

		void OnMessage_PktRegisterReq(int32_t nChannelInfo, int32_t nSrcPeerInfo, S2SPacket::PktRegisterReq* pkt);
	private:
		std::queue<uint8_t> m_queue_gateconnIds;
		std::queue<uint8_t> m_queue_gameconnIds;
		std::queue<uint8_t> m_queue_worldconnIds;
	};
}