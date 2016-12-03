#include "PacketDispatcher.h"

using namespace TerraX;

bool PacketDispatcher::DeliverPacket(NetChannelPtr& rChannel, int32_t nFromGuestID, const std::string& strMsgType, const char* pBuffer, const int nBufferSize)
{
	for (auto& it : m_mapCallBacks) {
		if (it.first->name() == strMsgType || it.first->full_name() == strMsgType) {
			const auto* pDesc = gpb::DescriptorPool::generated_pool()->FindMessageTypeByName(it.first->full_name());
			const auto* pProtoType = gpb::MessageFactory::generated_factory()->GetPrototype(pDesc);
			std::unique_ptr<gpb::Message> pMsg(pProtoType->New()); //return a new object;
			if (!pMsg->ParseFromArray(pBuffer, nBufferSize)) {
				return false; //parse error!
			}
			if (nFromGuestID == 0)
			{
				(*(it.second))(rChannel, *(pMsg.get()));
			}
			else
			{
				(*(it.second))(rChannel, nFromGuestID, *(pMsg.get()));
			}
			return true;
		}
	}
	return false;
	// packet handler not find! 
	// if you want to process the unknown packet, you can add a default handler.
}