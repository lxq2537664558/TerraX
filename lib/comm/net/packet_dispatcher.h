#pragma once

#include <functional>
#include <unordered_map>
#include "base/types.h"
#include "base/event_dynamic2.h"
#include "net_define.h"

namespace google
{
    namespace protobuf
    {
        class Descriptor;
    }
}

namespace terra
{
    namespace gpb = google::protobuf;
    // learn how to use std::forward
    class PacketDispatcher
    {
        MAKE_INSTANCE(PacketDispatcher);
		DISABLE_COPY(PacketDispatcher);
	private:
		std::unordered_map<const gpb::Descriptor*, std::unique_ptr<event_dynamic2> > packet_handlers_;
    public:
        PacketDispatcher() = default;

        template <class Packet>
        void RegPacketHandler(std::unique_ptr<event_dynamic2>&& ev)
        {
            assert(packet_handlers_.find(Packet::descriptor()) == packet_handlers_.end());
            packet_handlers_[Packet::descriptor()] = std::move(ev);
        }
        bool DeliverPacket(TcpConnection* conn, int32_t avatar_id, const char* msg_name,
                           const char* msg_data, const int msg_data_size);

    };

#define REG_PACKET_HANDLER_ARG1(packet_type, classobj, foo)                                  \
    \
PacketDispatcher::GetInstance()                                                              \
        .RegPacketHandler<packet_type>(std::unique_ptr<event_dynamic2>(new event_dynamic2( \
            std::function<void(packet_type*)>([classobj](packet_type* p) { classobj->foo(p); }))));

#define REG_PACKET_HANDLER_ARG2(packet_type, classobj, foo)                                                  \
    \
PacketDispatcher::GetInstance()                                                                              \
        .RegPacketHandler<packet_type>(                                                                      \
            std::unique_ptr<event_dynamic2>(new event_dynamic2(std::function<void(int32_t, packet_type*)>( \
                [classobj](int32_t n, packet_type* p) { classobj->foo(n, p); }))));

#define REG_PACKET_HANDLER_ARG3(packet_type, classobj, foo)                           \
    \
PacketDispatcher::GetInstance()                                                       \
        .RegPacketHandler<packet_type>(std::unique_ptr<event_dynamic2>(              \
            new event_dynamic2(std::function<void(TcpConnection*, int32_t, packet_type*)>( \
                [classobj](TcpConnection* conn, int32_t n, packet_type* p) { classobj->foo(conn, n, p); }))));
}