#pragma once
#include "ComDef.h"
#include "NetDefine.h"
#include <cassert>

namespace S2SPacket
{
	class PktRegisterReq;
}

namespace TerraX
{
	class CenterServer;
	class NetChannel;

	class ConnectionManager
	{
		DISABLE_COPY(ConnectionManager);
		MAKE_INSTANCE(ConnectionManager);
	public:
		ConnectionManager(); 
	private:

		void OnMessage_PktRegisterReq(uint16_t channel_index, int32_t nSrcPeerInfo, S2SPacket::PktRegisterReq* pkt);
	};
}