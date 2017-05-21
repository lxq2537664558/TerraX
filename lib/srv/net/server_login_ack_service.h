#pragma once
#include "base/types.h"
#include <queue>
#include <vector>
#include "comm/proto/server_server.pb.h"
#include "server_accept_service.h"
#include "server_table.h"

namespace terra
{
	class TcpConnection;

	class ServerLoginAckService
	{
	private:
		ServerAcceptService& accept_service_;
		const PeerType_t& kSelfPeer;
		const int max_conns_;
		std::queue<int> server_ids_;
	public:
		ServerLoginAckService(ServerAcceptService& accept_service, PeerType_t peer, int max_conns);
		~ServerLoginAckService();
		void OnLoginOut(TcpConnection* conn);
	private:
		void OnServerTableEvent(const std::vector<Net_Object>& objs, const Net_Object& net_obj, ServerTableEvent_t ev);

		void OnMessage_RegisterSW(TcpConnection* conn, int32_t avatar_id, packet_ss::MsgRegisterSW* msg);
	};
}