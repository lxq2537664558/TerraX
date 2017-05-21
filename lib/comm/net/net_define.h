#pragma once

#include <functional>
#include <memory>
#include <event2/buffer.h>
#include "base/types.h"

namespace terra
{

	const int WORD_SERVER_ID = 0;
	class TcpConnection;
	enum class ConnState_t;


	using SocketEventCB = std::function<void(TcpConnection*, ConnState_t)>;
	using MessageEventCB = std::function<void(TcpConnection*, struct evbuffer*)>;

    // when we need to logout, we should first tell the gate server
    enum class KickOutReason_t {
        eNormalExit,
    };

    enum class ConnState_t {
        DISCONNECTED,
        CONNECTING,
        CONNECTED,  // register
        DISCONNECTING
    };

    enum class PeerType_t {
        UNDEFINE = 0,
        CLIENT = 1,

        GATESERVER = 2,
        LOGINSERVER = 3,
        MASTERSERVER = 4,
        NODESERVER = 5,
        QUEUESERVER = 6,
        WORLDSERVER = 7,

    };
	class NetHelper
	{
	public:
		static const char* ServerName(PeerType_t peer_type)
		{
			switch (peer_type) {
			case PeerType_t::CLIENT:
				return "client";
			case PeerType_t::GATESERVER:
				return "gate";
			case PeerType_t::LOGINSERVER:
				return "login";
			case PeerType_t::MASTERSERVER:
				return "master";
			case PeerType_t::NODESERVER:
				return "node";
			case PeerType_t::QUEUESERVER:
				return "queue";
			case PeerType_t::WORLDSERVER:
				return "world";
			default:
				break;
			}
			return "unknown host";
		}
	};
}