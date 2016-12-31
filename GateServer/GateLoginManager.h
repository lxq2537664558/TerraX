#pragma once
#include "ComDef.h"

namespace C2SPacket
{
	class PktGameLoginReq;
}
namespace TerraX
{
	class GateLoginManager
	{
		NOCOPY(GateLoginManager);
		MAKEINSTANCE(GateLoginManager);
	public:
		GateLoginManager();
		~GateLoginManager() = default;

	private:
		void RegPacketHandlerFunction();

		void OnMessage_PktGameLoginReq(int guest_id, C2SPacket::PktGameLoginReq* pkt);
	};
}