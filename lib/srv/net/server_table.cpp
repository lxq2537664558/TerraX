#include "server_table.h"
#include <algorithm>
using namespace terra;

Net_Object* ServerTable::GetNetObjectByServerID(int server_id)
{
	for (auto& val : servers_)
	{
		if (val.server_id_ == server_id)
		{
			return &val;
		}
	}
	return nullptr;
}

Net_Object* ServerTable::GetNetObjectByFD(int fd)
{
	for (auto& val : servers_)
	{
		if (val.fd_ == fd)
		{
			return &val;
		}
	}
	return nullptr;
}

Net_Object* ServerTable::GetNetObjectByConn(TcpConnection* conn)
{
	for (auto& val : servers_)
	{
		if (val.conn_ == conn)
		{
			return &val;
		}
	}
	return nullptr;
}

void ServerTable::AddServerInfo(PeerType_t peer, int server_id, const char* listen_ip, int listen_port, TcpConnection* conn)
{
	Net_Object net_obj(peer, server_id, listen_ip, listen_port, conn->get_fd(), conn);
	servers_.push_back(net_obj);
	if (cb_)
	{
		cb_(servers_, net_obj, ServerTableEvent_t::NETOBJECT_ADD);
	}
}

void ServerTable::RemoveByServerID(int server_id)
{
	auto iter = std::find_if(servers_.begin(), servers_.end(),
		[server_id](const Net_Object& obj) {
		return obj.server_id_ == server_id;
	});
	if(iter != servers_.end())
	{
		servers_.erase(iter);
	}
	cb_(servers_, *iter, ServerTableEvent_t::NETOBJECT_REMOVE);
}

void ServerTable::RemoveByFD(int fd)
{
	auto iter = std::find_if(servers_.begin(), servers_.end(),
		[fd](const Net_Object& obj) {
		return obj.fd_ == fd;
	});
	if (iter != servers_.end())
	{
		servers_.erase(iter);
	}
	cb_(servers_, *iter, ServerTableEvent_t::NETOBJECT_REMOVE);
}

void ServerTable::RemoveByConn(TcpConnection* conn)
{
	auto iter = std::find_if(servers_.begin(), servers_.end(),
		[conn](const Net_Object& obj) {
		return obj.conn_ == conn;
	});
	if (iter != servers_.end())
	{
		servers_.erase(iter);
	}
	cb_(servers_, *iter, ServerTableEvent_t::NETOBJECT_REMOVE);
}