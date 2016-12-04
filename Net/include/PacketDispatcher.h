#pragma once
#include <map>
#include <functional>
#include <google/protobuf/message.h>
#include "NetDefine.h"
#include "ComDef.h"

namespace TerraX
{
	class NetChannel;
	namespace gpb = google::protobuf;
	class IPacketFunctor
	{
	public:
		IPacketFunctor() = default;
		virtual void operator()(NetChannelPtr& channel, gpb::Message& arg) {}
		virtual void operator()(NetChannelPtr& channel, int32_t fromid, gpb::Message& arg) {}
	};

	template<typename Packet>
	class PacketFunctor : public IPacketFunctor
	{
		using PacketCB = std::function<void(NetChannelPtr&, Packet&)>;
	public:
		explicit PacketFunctor(PacketCB cb) {
			this->cb = cb;
		}
		void operator()(NetChannelPtr& channel, gpb::Message& msg) override final {
			cb(channel, static_cast<Packet&>(msg));
		}
	private:
		PacketCB cb;
	};

	template<typename Packet, typename int32_t>
	class PacketFunctorArg1 : public IPacketFunctor
	{
		using GuestPacketCB = std::function<void(NetChannelPtr&, Packet&, int32_t)>;
	public:
		PacketFunctorArg1(GuestPacketCB gcb) {
		this->gcb = gcb;
		}
		void operator()(NetChannelPtr& channel, int32_t fromid, gpb::Message& msg) override final {
			gcb(channel, fromid, static_cast<Packet&>(msg));
		}
	private:
		GuestPacketCB gcb;
	};



	class PacketDispatcher
	{
		MAKEINSTANCE(PacketDispatcher);
	public:
		PacketDispatcher() = default;

		template <class Packet>
		void RegPacketHandler(IPacketFunctor* pMsg) {
			assert(m_mapCallBacks.find(Packet::descriptor()) == m_mapCallBacks.end());
			m_mapCallBacks[Packet::descriptor()] = pMsg;
		}

		bool DeliverPacket(NetChannelPtr& rChannel, int32_t nFromGuestID, const std::string& strMsgType, const char* pBuffer, const int nBufferSize);
	private:
		std::map<const google::protobuf::Descriptor*, IPacketFunctor* > m_mapCallBacks;
	};
}