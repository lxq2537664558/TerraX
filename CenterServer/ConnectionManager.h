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
	class CenterServer;
	class NetChannel;
	class ConnectionManager
	{
		NOCOPY(ConnectionManager);
	public:
		ConnectionManager(CenterServer& cs); 

		void OnMessage_Register(NetChannel& channel, PktRegisterServer& pkt); 

	private:
		CenterServer& server;
		// maybe we should use shared_ptr?
	};
}