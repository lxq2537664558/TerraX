#pragma once
#include "ComDef.h"
#include <cstdint>
#include <cassert>
#include <memory>
#include <functional>
namespace TerraX
{
	class NetChannel;
	typedef std::shared_ptr<NetChannel> NetChannelPtr;

	enum class NetEvent_t
	{
		eConnected,
		eConnectFailed,
		eDisconnected,
	};
	using NetEvent_CB = std::function<void(NetChannelPtr&, NetEvent_t)>;


	enum class ConnState_t
	{
		eDisconnected,
		eConnecting,
		eConnected,
		eDisconnecting
	};

	enum class PeerType_t
	{
		undefine = 0,
		client = 1,

		centerserver = 1 << 1,
		gateserver = 1 << 2,
		gameserver = 1 << 3,
		loginserver = 1 << 4,
		worldserver = 1 << 5,

		peer_count = 1 << 6,
		all_peer = 0xFFFF,

	};
	/// server info layout
	/// @peer_type: (uint16_t) 0~peer_count(if 0: unregister , if 0xFFFF : broadcast all peer)
	/// @channel_index: (uint16_t) 0x0000~0xFFFF-1 (if 0; unregister, if 0xFFFF boradcast all channel that connected to peer)
	/// +-------------------+------------------+
	/// |     peer_type     |  channel_index   |
	/// +-------------------+------------------+
	/// |                   |                  |              
	/// 0        <=       16bits     <=      32bits    
	/// @end
	class PeerInfo
	{
	public:
		PeerInfo() = default;
		explicit PeerInfo(PeerType_t ePeerType) : peer_type(uint16_t(ePeerType)){
		}
		void parse(int32_t server_info) {
			peer_type = (server_info & 0xFFFF0000) >> 16;
			channel_index = server_info & 0x0000FFFF;
			assert(peer_type > (uint16_t)PeerType_t::undefine && peer_type < (uint16_t)PeerType_t::peer_count);
		}
		int32_t serialize() {
			return (peer_type << 16) + channel_index;
		}
		const char* server_name() {
			switch (PeerType_t(peer_type))
			{
			case PeerType_t::client:				return "client";
			case PeerType_t::centerserver:			return "center";
			case PeerType_t::gateserver:			return "gate";
			case PeerType_t::gameserver:			return "game";
			case PeerType_t::loginserver:			return "login";
			case PeerType_t::worldserver:			return "world";
			default:
				break;
			}
			return "unknown host";
		}
	public:
		uint16_t peer_type{ uint16_t(PeerType_t::undefine) };
		uint16_t channel_index{ 0 };
	};
}