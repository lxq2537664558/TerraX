#pragma once
#include <cstdint>
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
	/// |     peer_type     |     peer_index   |  channel_index   |	
	/// +-------------------+------------------+------------------+
	/// |                   |                  |                  | 
	/// 0        <=       8bits     <=       8bits       <=     16bits 
	/// @end
	struct ServerInfo_t
	{
		uint8_t peer_type;
		uint8_t peer_index;
		uint16_t channel_index;
		void parse(int32_t server_info) {
			peer_type = server_info & 0xFF000000;
			peer_index = server_info & 0x00FF0000;
			channel_index = server_info & 0x0000FFFF;
		}
		int serialize() {
			return (peer_type << 24) + (peer_index << 16) + channel_index;
		}
	};
}