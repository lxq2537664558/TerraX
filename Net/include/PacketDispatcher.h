#pragma once
#include <functional>
#include <unordered_map>
#include "ComDef.h"
#include "NetDefine.h"
#include "event_dynamic2.h"

namespace google
{
	namespace protobuf {
		class Descriptor;
	}
}

namespace TerraX
{
    namespace gpb = google::protobuf;
    // learn how to use std::forward
    class PacketDispatcher
    {
        MAKE_INSTANCE(PacketDispatcher);

    public:
        PacketDispatcher() = default;

        template <class Packet>
        void RegPacketHandler(std::unique_ptr<events_dynamic2>&& pEvent)
        {
            assert(m_mapCallBacks.find(Packet::descriptor()) == m_mapCallBacks.end());
            m_mapCallBacks[Packet::descriptor()] = std::move(pEvent);
        }
        bool DeliverPacket(uint16_t channel_index, int32_t msg_owner_info, const std::string& msg_type_name,
                           const char* buffer, const int buffer_size);

    private:
        std::unordered_map<const gpb::Descriptor*, std::unique_ptr<events_dynamic2> > m_mapCallBacks;
    };

#define REG_PACKET_HANDLER_ARG1(packet_type, bind_function) \
    \
PacketDispatcher::GetInstance()                           \
        .RegPacketHandler<packet_type>(                   \
            std::unique_ptr<events_dynamic2>(new events_dynamic2(std::function<void(packet_type*)>(bind_function))));

#define REG_PACKET_HANDLER_ARG2(packet_type, bind_function) \
    \
PacketDispatcher::GetInstance()                           \
        .RegPacketHandler<packet_type>(                   \
            std::unique_ptr<events_dynamic2>(new events_dynamic2(std::function<void(int32_t, packet_type*)>(bind_function))));

#define REG_PACKET_HANDLER_ARG3(packet_type, bind_function) \
    \
PacketDispatcher::GetInstance()                           \
        .RegPacketHandler<packet_type>(                   \
            std::unique_ptr<events_dynamic2>(new events_dynamic2(std::function<void(uint16_t, int32_t, packet_type*)>(bind_function))));
}