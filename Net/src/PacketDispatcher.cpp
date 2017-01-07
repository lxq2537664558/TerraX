#include "PacketDispatcher.h"
#include <google/protobuf/message.h>

using namespace TerraX;

bool PacketDispatcher::DeliverPacket(uint16_t channel_index, int32_t msg_owner_info,
                                      const std::string& msg_type_name, const char* buffer,
                                      const int buffer_size)
{
    for (auto& it : m_mapCallBacks) {
        if (it.first->name() == msg_type_name || it.first->full_name() == msg_type_name) {
            const auto* pDesc =
                gpb::DescriptorPool::generated_pool()->FindMessageTypeByName(it.first->full_name());
            const auto* pProtoType = gpb::MessageFactory::generated_factory()->GetPrototype(pDesc);
            std::unique_ptr<gpb::Message> pMsg(pProtoType->New());  // return a new object;
            assert(pMsg);
            if (!pMsg->ParseFromArray(buffer, buffer_size)) {
                return false;  // parse error!
            }
            auto& pEvent = it.second;
            if (pEvent->args_size() == 1) {
                (*(pEvent.get()))(pMsg.get());
            } else if (pEvent->args_size() == 2) {
                (*(pEvent.get()))(msg_owner_info, pMsg.get());
            } else if (pEvent->args_size() == 3) {
                (*(pEvent.get()))(channel_index, msg_owner_info, pMsg.get());
            }
			else {
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