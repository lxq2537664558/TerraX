#pragma once

#include "base/types.h"
#include "server_conn_service.h"
#include "comm/proto/server_server.pb.h"

namespace terra
{
	class TcpConnection;
	class ServerLoginReqService
	{
	private:
		ServerConnService& conn_service_;
		const PeerType_t& kSelfPeer;
	public:
		ServerLoginReqService(ServerConnService& conn_service, PeerType_t peer);
		~ServerLoginReqService() = default;

		void Login2World(TcpConnection* conn);

		void OnLoginOut(TcpConnection* conn);
	private:
		void OnMessage_RegisterWS(packet_ss::MsgRegisterWS* msg);
		void OnMessage_ServerInfoWS(packet_ss::MsgServerInfoWS* msg);
	};
}