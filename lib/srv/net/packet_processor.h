#pragma once

#include <event2/bufferevent.h>
#include "base/types.h"
#include "comm/net/net_define.h"
#include <google/protobuf/message.h>
namespace terra
{
	class PacketProcessor
	{
		DISABLE_COPY(PacketProcessor);
		MAKE_INSTANCE(PacketProcessor);
	public:
		PacketProcessor(){}
		~PacketProcessor(){}

		void SendPacket2World(google::protobuf::Message& msg);
		void SendPacket(int server_id, google::protobuf::Message& msg);
		void SendPacket(TcpConnection* conn, google::protobuf::Message& msg);

		void ProcessServerPacket(TcpConnection* conn, struct evbuffer* evbuf);
		
	};
}