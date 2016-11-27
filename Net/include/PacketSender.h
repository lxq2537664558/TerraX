#pragma once
#include "ComDef.h"
#include "NetDefine.h"

namespace TerraX
{
	//?template<class T>
	class PacketSender final
	{
		NOCOPY(PacketSender);
		MAKEINSTANCE(PacketSender);
	public:
		PacketSender();
		~PacketSender() = default;
		void SendPacket(){}
		void SendPacket(int32_t server) {}

	private:
		NetChannelPtr m_pSender;
	};

}