#pragma once
#include "ComDef.h"
#include "NetChannel.h"
#include "proto/server_packet.pb.h"
#include "NetDefine.h"

using namespace ServerPacket;
namespace TerraX
{
	class WorldServer 
	{
		NOCOPY(WorldServer);
	private:
		PeerInfo m_PeerInfo{ PeerType_t::worldserver };
	};
}