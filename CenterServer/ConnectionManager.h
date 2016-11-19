#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include "proto/server_packet.pb.h"
#include <array>
#include <queue>
#include <map>
#include <cassert>

using namespace ServerPacket;
namespace TerraX
{
	//you can load these info from config file;
	//index 0 means not registered,so we start with index 1;
	const uint8_t Max_WorldConnection_Count = 2; 
	const uint8_t Max_GateConnection_Count = 33;
	const uint8_t Max_GameConnection_Count = 4;
	class CenterServer;
	class NetChannel;
	class ConnectionManager
	{
		NOCOPY(ConnectionManager);
	public:
		ConnectionManager(CenterServer& cs); 

		void OnMessage_RegisterServer(NetChannel& channel, PktRegisterServer& pkt); 

		void UnregisterServer(NetChannel* pChannel);
	private:
		CenterServer& server;
		std::array<std::queue<uint8_t>, int32_t(PeerType_t::peer_count)> m_freeindexes;
		std::map<int32_t, NetChannel*> m_mapRegisterChannels;
		// maybe we should use shared_ptr?
	};
}