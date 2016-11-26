#pragma once
#include "ComDef.h"

namespace TerraX
{
	class PacketSender final
	{
		NOCOPY(PacketSender);
		MAKEINSTANCE(PacketSender);
	public:
		PacketSender() = default;
		~PacketSender() = default;
		void SendPacket(){}
	};

}