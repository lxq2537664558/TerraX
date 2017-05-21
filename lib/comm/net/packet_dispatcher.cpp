#include "packet_dispatcher.h"

#include <google/protobuf/message.h>

using namespace terra;

bool PacketDispatcher::DeliverPacket(TcpConnection* conn, int32_t avatar_id, const char* msg_name,
                                     const char* msg_data, const int msg_data_size)
{
    for (auto& val : packet_handlers_) {
        if (val.first->name() == msg_name || val.first->full_name() == msg_name) {
            const auto* desc =
                gpb::DescriptorPool::generated_pool()->FindMessageTypeByName(val.first->full_name());
            const auto* proto_type = gpb::MessageFactory::generated_factory()->GetPrototype(desc);
            std::unique_ptr<gpb::Message> msg(proto_type->New());  // return a new object;
            assert(msg);
            if (!msg->ParseFromArray(msg_data, msg_data_size)) {
                assert(0);
                return false;  // parse error!
            }
            const auto& ev = val.second;
            if (ev->args_size() == 1) {
                (*(ev.get()))(msg.get());
            } else if (ev->args_size() == 2) {
                (*(ev.get()))(avatar_id, msg.get());
            } else if (ev->args_size() == 3) {
                (*(ev.get()))(conn, avatar_id, msg.get());
            } else {
                assert(1);
            }
            return true;
        }
    }
    assert(0);
    // packet handler not find!
    // if you want to process the unknown packet, you can add a default handler.
    return false;
}