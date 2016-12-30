#pragma once
#include <cassert>
#include <cstdint>
#include <functional>
#include <memory>
#include "ComDef.h"
namespace TerraX
{
    class NetChannel;
    typedef std::shared_ptr<NetChannel> NetChannelPtr;

    enum class NetEvent_t {
        eConnected,
        eConnectFailed,
        eDisconnected,
    };
    using NetEvent_CB = std::function<void(NetChannelPtr&, NetEvent_t)>;

	//when we need to logout, we should first tell the gate server
	enum class KickOutReason_t {
		eNormalExit,
	};

    enum class ConnState_t {
        eDisconnected,
        eConnecting,
        eConnected,  // register
        eDisconnecting
    };

    enum class PeerType_t {
        undefine = 0,
        client = 1,

        centerserver = 2,
        gateserver = 3,
        gameserver = 4,
        loginserver = 5,
        worldserver = 6,

        peer_count = 7,
        all_peer = 0xFF,

    };
    /// server info layout
    /// @peer_type: (uint8_t) 0~peer_count(if 0: unregister , if 0xFF :
    /// broadcast
    /// all peer)
    /// @peer_index: (uint8_t) 0~peer_index(if 0: unregister , if 0xFF :
    /// broadcast
    /// current peer)
    /// @channel_index: (uint16_t) 0x0000~0xFFFF-1 (if 0; unregister, if 0xFFFF
    /// boradcast all channel that connected to peer)
    /// +-------------------+------------------+------------------+
    /// |     peer_type     |  channel_index   |  channel_index   |
    /// +-------------------+------------------+------------------+
    /// |                   |                  |
    /// 0        <=        8bits     <=      8bits     <=      32bits
    /// @end
    class PeerInfo
    {
    public:
        PeerInfo() = default;
        explicit PeerInfo(PeerType_t ePeerType) : peer_type(ePeerType) {}
        explicit PeerInfo(int32_t peer_info) { parse(peer_info); }
        void parse(int32_t server_info)
        {
            peer_type = PeerType_t((server_info & 0xFF000000) >> 24);
            peer_index = (server_info & 0x00FF0000) >> 16;
            channel_index = server_info & 0x0000FFFF;
        }
        int32_t serialize() { return (uint8_t(peer_type) << 24) + (peer_index << 16) + channel_index; }
        const char* server_name()
        {
            switch (peer_type) {
                case PeerType_t::client:
                    return "client";
                case PeerType_t::centerserver:
                    return "center";
                case PeerType_t::gateserver:
                    return "gate";
                case PeerType_t::gameserver:
                    return "game";
                case PeerType_t::loginserver:
                    return "login";
                case PeerType_t::worldserver:
                    return "world";
                default:
                    break;
            }
            return "unknown host";
        }

    public:
        PeerType_t peer_type{PeerType_t::undefine};
        uint8_t peer_index{0};
        uint16_t channel_index{0};
    };
}