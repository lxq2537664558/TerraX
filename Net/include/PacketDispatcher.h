#pragma once
#include <map>
#include <functional>
#include <google/protobuf/message.h>
#include "ComDef.h"

namespace TerraX
{
	class NetChannel;
	namespace gpb = google::protobuf;
	class IPacketFunctor
	{
	public:
		IPacketFunctor() = default;
		virtual void operator()(NetChannel& channel, gpb::Message& arg) = 0;
	};

	template<typename Packet>
	class PacketFunctor : public IPacketFunctor
	{
		using PacketCB = std::function<void(NetChannel&, Packet&)>;
	public:
		PacketFunctor(PacketCB cb) {
			this->cb = cb;
		}
		void operator()(NetChannel& channel, gpb::Message& msg) override {
			cb(channel, static_cast<Packet&>(msg));
		}
	private:
		PacketCB cb;
	};

	class PacketDispatcher
	{
		MAKEINSTANCE(PacketDispatcher);
	public:
		PacketDispatcher() = default;

		template <class Packet>
		void RegPacketHandler(IPacketFunctor* pMsg) {
			m_mapCallBacks[Packet::descriptor()] = pMsg;
		}

		bool DeliverPacket(NetChannel& rChannel, const std::string& strMsgType, const char* pBuffer, const int nBufferSize);
	private:
		std::map<const google::protobuf::Descriptor*, IPacketFunctor* > m_mapCallBacks;
	};
}