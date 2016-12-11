#include "PacketDispatcher.h"

using namespace TerraX;

bool PacketDispatcher::DeliverPacket(NetChannelPtr& pChannel, int32_t nFromPeerInfo, const std::string& strMsgType, const char* pBuffer, const int nBufferSize)
{
	for (auto& it : m_mapCallBacks) {
		if (it.first->name() == strMsgType || it.first->full_name() == strMsgType) {
			const auto* pDesc = gpb::DescriptorPool::generated_pool()->FindMessageTypeByName(it.first->full_name());
			const auto* pProtoType = gpb::MessageFactory::generated_factory()->GetPrototype(pDesc);
			std::unique_ptr<gpb::Message> pMsg(pProtoType->New()); //return a new object;
			if (!pMsg->ParseFromArray(pBuffer, nBufferSize)) {
				return false; //parse error!
			}
			{
				(*(it.second))(pChannel, nFromPeerInfo, *(pMsg.get()));
			}
			return true;
		}
	}
	assert(0);
	return false;
	// packet handler not find! 
	// if you want to process the unknown packet, you can add a default handler.
}