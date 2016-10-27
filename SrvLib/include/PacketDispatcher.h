#pragma once
#include <map>
#include <functional>
#include "google/protobuf/message.h"
#include "ComDef.h"

namespace TerraX
{
	namespace gpb = google::protobuf;
	class IPacketFunctor
	{
	public:
		IPacketFunctor() = default;
		virtual void operator()(gpb::Message& arg) = 0;
	};

	template<typename Packet>
	class PacketFunctor : public IPacketFunctor
	{
		using PacketCB = std::function<void(Packet&)>;
	public:
		PacketFunctor(PacketCB cb)
		{
			this->cb = cb;
		}
		void operator()(gpb::Message& msg) override
		{
			cb(static_cast<Packet&>(msg));
		}
	private:
		PacketCB cb;
	};

	class PacketDispatcher
	{
	public:
		PacketDispatcher() = default;
		template <class Packet>
		void RegPacketHandler(IPacketFunctor* pMsg)
		{
			m_mapCallBacks[Packet::descriptor()] = pMsg;
		}


		void DeliverPacket(const std::string& strMsgType, const char* pBuffer, const int nBufferSize)
		{
			for (auto& it : m_mapCallBacks) {
				if (it.first->name() == strMsgType || it.first->full_name() == strMsgType) {
					const auto* pDesc = gpb::DescriptorPool::generated_pool()->FindMessageTypeByName(it.first->full_name());
					const auto* pProtoType = gpb::MessageFactory::generated_factory()->GetPrototype(pDesc);
					std::unique_ptr<gpb::Message> pMsg(pProtoType->New()); //return a new object;
					pMsg->ParseFromArray(pBuffer, nBufferSize);
					(*(it.second))(*(pMsg.get()));
					return;
				}
			}
			// packet handler not find! 
			// if you want to process the unknown packet, you can add a default handler.
		}
	private:
		std::map<const google::protobuf::Descriptor*, IPacketFunctor* > m_mapCallBacks;
	};
}