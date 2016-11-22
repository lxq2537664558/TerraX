#pragma once
#include "ComDef.h"
#include <cstdint>
#include <cassert>
namespace TerraX
{
	enum class PeerType_t
	{
		undefine = 0,
		client = 1,

		centerserver,
		gateserver,
		gameserver,
		loginserver,
		worldserver,

		peer_count,

		allpeer = 0xFF,
	};
	/// server info layout
	/// @peer_type: (uint8_t) 0~peer_count(if 0: unregister , if 0xFF : broadcast all peer)
	/// @peer_index: (uint8_t) 0~0xFF-1 (if 0: unregister, if 0xFF: broadcast current peer)
	/// @channel_index: (uint16_t) 0x0000~0xFFFF-1 (if 0; unregister, if 0xFFFF boradcast all channel that connected to peer)
	/// +-------------------+------------------+------------------+
	/// |     peer_type     |     peer_index   |  channel_index   |	
	/// +-------------------+------------------+------------------+
	/// |                   |                  |                  | 
	/// 0        <=       8bits     <=       8bits       <=     16bits 
	/// @end
	class PeerInfo
	{
	public:
		PeerInfo() = default;
		explicit PeerInfo(PeerType_t ePeerType) : peer_type(uint8_t(ePeerType)){
		}
		void parse(int32_t server_info) {
			peer_type = (server_info & 0xFF000000) >> 24;
			peer_index = (server_info & 0x00FF0000) >> 16;
			channel_index = server_info & 0x0000FFFF;
			assert(peer_type > (uint8_t)PeerType_t::undefine && peer_type < (uint8_t)PeerType_t::peer_count);
		}
		int32_t serialize() {
			return (peer_type << 24) + (peer_index << 16) + channel_index;
		}
	public:
		uint8_t peer_type{ uint8_t(PeerType_t::undefine) };
		uint8_t peer_index{ 0 };
		uint16_t channel_index{ 0 };
	};
}