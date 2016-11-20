#pragma once
#include "ComDef.h"
#include <cstdint>
#include <cassert>
namespace TerraX
{
	enum class PeerType_t
	{
		undefine = 0,
		client,
		centerserver,
		gateserver,
		gameserver,
		loginserver,
		worldserver,
		peer_count,

		allpeer = 0xFF,
	};
	/// server info layout
	/// @info int32_t
	/// +-------------------+------------------+------------------+
	/// |     peer_type     |     peer_index   |  client_index   |	
	/// +-------------------+------------------+------------------+
	/// |                   |                  |                  | 
	/// 0        <=       8bits     <=       8bits       <=     16bits 
	/// @end
	class PeerInfo
	{
		NOCOPY(PeerInfo);
	public:
		PeerInfo() = default;
		explicit PeerInfo(PeerType_t ePeerType) : peer_type(uint8_t(ePeerType)){
		}
		void parse(int32_t server_info) {
			peer_type = (server_info & 0xFF000000) >> 24;
			peer_index = (server_info & 0x00FF0000) >> 16;
			client_index = server_info & 0x0000FFFF;
			assert(peer_type > (uint8_t)PeerType_t::undefine && peer_type < (uint8_t)PeerType_t::peer_count);
		}
		int32_t serialize() {
			return (peer_type << 24) + (peer_index << 16) + client_index;
		}
	public:
		uint8_t peer_type{ uint8_t(PeerType_t::undefine) };
		uint8_t peer_index{ 0 };
		uint16_t client_index{ 0 };
	};
}