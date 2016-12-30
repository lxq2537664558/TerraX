#include "PacketDispatcher.h"
#include <google/protobuf/message.h>

using namespace TerraX;

bool PacketDispatcher::DeliverPacket(int32_t nChannelInfo, int32_t nMsgOwnerInfo,
                                      const std::string& strMsgType, const char* pBuffer,
                                      const int nBufferSize)
{
    for (auto& it : m_mapCallBacks) {
        if (it.first->name() == strMsgType || it.first->full_name() == strMsgType) {
            const auto* pDesc =
                gpb::DescriptorPool::generated_pool()->FindMessageTypeByName(it.first->full_name());
            const auto* pProtoType = gpb::MessageFactory::generated_factory()->GetPrototype(pDesc);
            std::unique_ptr<gpb::Message> pMsg(pProtoType->New());  // return a new object;
            assert(pMsg);
            if (!pMsg->ParseFromArray(pBuffer, nBufferSize)) {
                return false;  // parse error!
            }
            auto& pEvent = it.second;
            if (pEvent->args_size() == 1) {
                (*(pEvent.get()))(pMsg.get());
            } else if (pEvent->args_size() == 2) {
                (*(pEvent.get()))(nMsgOwnerInfo, pMsg.get());
            } else if (pEvent->args_size() == 3) {
                (*(pEvent.get()))(nChannelInfo, nMsgOwnerInfo, pMsg.get());
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