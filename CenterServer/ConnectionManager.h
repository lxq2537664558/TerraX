#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include "proto/server_server.pb.h"
#include <array>
#include <queue>
#include <map>
#include <cassert>

using namespace S2SPacket;
namespace TerraX
{
	//you can load these info from config file;
	//index 0 means not registered,so we start with index 1;
	const uint8_t Max_WorldConnection_Count = 2; 
	const uint8_t Max_GateConnection_Count = 4;
	const uint8_t Max_GameConnection_Count = 4;
	class CenterServer;
	class NetChannel;
	class ConnectionManager
	{
		NOCOPY(ConnectionManager);
	public:
		ConnectionManager(CenterServer& cs); 

		void OnMessage_Register(NetChannel& channel, PktRegisterServer& pkt); 

		void UnRegister(int32_t peer_info);
	private:
		CenterServer& server;
		std::array<std::queue<uint8_t>, int32_t(PeerType_t::peer_count)> m_freeindexes;
		// maybe we should use shared_ptr?
	};
}