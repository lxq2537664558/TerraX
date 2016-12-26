#pragma once
#include <google/protobuf/message.h>
#include <functional>
#include <map>
#include "ComDef.h"
#include "NetDefine.h"
#include "event_dynamic2.h"

namespace TerraX
{
    namespace gpb = google::protobuf;
    // learn how to use std::forward
    class PacketDispatcher
    {
        MAKEINSTANCE(PacketDispatcher);

    public:
        PacketDispatcher() = default;

        template <class Packet>
        void RegPacketHandler(events_dynamic2* pEvent)
        {
            assert(m_mapCallBacks.find(Packet::descriptor()) == m_mapCallBacks.end());
            m_mapCallBacks[Packet::descriptor()].reset(std::move(pEvent));
        }
        bool DeliverPacket(int32_t nChannelInfo, int32_t nMsgOwnerInfo, const std::string& strMsgType,
                           const char* pBuffer, const int nBufferSize);

    private:
        std::map<const google::protobuf::Descriptor*, std::unique_ptr<events_dynamic2> > m_mapCallBacks;
    };

#define RegPacketHandler_Arg1(packet_type, bind_function) \
    \
PacketDispatcher::GetInstance()                           \
        .RegPacketHandler<packet_type>(                   \
            new events_dynamic2(std::function<void(packet_type*)>(bind_function)));

#define RegPacketHandler_Arg2(packet_type, bind_function) \
    \
PacketDispatcher::GetInstance()                           \
        .RegPacketHandler<packet_type>(                   \
            new events_dynamic2(std::function<void(int32_t, packet_type*)>(bind_function)));

#define RegPacketHandler_Arg3(packet_type, bind_function) \
    \
PacketDispatcher::GetInstance()                           \
        .RegPacketHandler<packet_type>(                   \
            new events_dynamic2(std::function<void(int32_t, int32_t, packet_type*)>(bind_function)));
}