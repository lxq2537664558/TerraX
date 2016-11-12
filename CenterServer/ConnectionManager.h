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
	const int Max_WorldConnection_Count = 1;
	const int Max_GateConnection_Count = 32;
	const int Max_GameConnection_Count = 32;
	class CenterServer;
	class NetChannel;
	class ConnectionManager
	{
		NOCOPY(ConnectionManager);
	public:
		ConnectionManager(CenterServer& cs); 

		void OnMessage_RegisterServer(NetChannel& channel, PktRegisterServer& pkt); 
		//void OnMessage_UnRegisterServer()

		void UnregisterServer(NetChannel* pChannel);
	private:
		CenterServer& server;
		std::array<std::queue<uint32_t>, int32_t(PeerType_t::peer_count)> m_freeindexes;
		std::map<uint32_t, NetChannel*> m_mapRegisterChannels;
	};
}