#pragma once
#include <functional>
#include <map>
#include <vector>
#include "base/types.h"
#include "comm/net/net_define.h"
#include "comm/net/tcp_connection.h"
namespace terra
{
    struct Net_Object {
        Net_Object(PeerType_t peer_type, int server_id, const char* listen_ip, int listen_port, int fd, TcpConnection* conn)
            : peer_type_(peer_type), 
			server_id_(server_id), 
			listen_port_(listen_port),
			fd_(conn->get_fd()), 
			conn_(conn)
        {
			assert(strlen(listen_ip) < 32);
			memcpy(listen_ip_, listen_ip, strlen(listen_ip));
        }

        PeerType_t peer_type_{PeerType_t::UNDEFINE};
        int server_id_{0};
		char listen_ip_[32]{ 0 };
		int listen_port_{ 0 };
        int fd_{0};
        TcpConnection* conn_{nullptr};
    };
    enum class ServerTableEvent_t {
        NETOBJECT_ADD,
        NETOBJECT_REMOVE,
    };
    using ServerTableEventCB =
        std::function<void(const std::vector<Net_Object>&, const Net_Object&, ServerTableEvent_t)>;
    class ServerTable
    {
        DISABLE_COPY(ServerTable);
        MAKE_INSTANCE(ServerTable);

    private:
		PeerType_t self_peer_{ PeerType_t::UNDEFINE };
		int self_server_id_{ -1 };
        std::vector<Net_Object> servers_;
        ServerTableEventCB cb_;

    public:
        ServerTable(){};
        ~ServerTable() = default;

		void InitSelfServerInfo(PeerType_t peer, int server_id)
		{ 
			self_peer_ = peer;
			self_server_id_ = server_id;
		}
        void SetServerTableEventCB(ServerTableEventCB cb) { cb_ = cb; }

        Net_Object* GetNetObjectByServerID(int server_id);
        Net_Object* GetNetObjectByFD(int fd);
        Net_Object* GetNetObjectByConn(TcpConnection* conn);

        void AddServerInfo(PeerType_t peer, int server_id, const char* ip, int port, TcpConnection* conn);
        void RemoveByServerID(int server_id);
        void RemoveByFD(int fd);
        void RemoveByConn(TcpConnection* conn);
    };
}