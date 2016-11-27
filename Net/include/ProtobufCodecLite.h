#pragma once

#include "ComDef.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <cassert>
#include <google/protobuf/message.h>
#include <zlib.h>
#include "NetChannel.h"

namespace TerraX
{
	namespace gpb =  google::protobuf;
	class ProtoBufCodecLite
	{
		NOCOPY(ProtoBufCodecLite)
	public:
		const static int header_size = sizeof(int);
		const static int checksum_size = sizeof(int);
		const static int max_message_size = 64 * 1024;

		enum class ErrorCode_t
		{
			eNoError,
			eInvalidLength,
			eCheckSumError,
			eInvalidNameLen,
			eUnknownMessage,
			eParseError,
		};
		void Send(const NetChannelPtr& channel, const gpb::Message& msg)
		{

		}
	};


	template<class T>
	class ProtobufCodecLiteT
	{
	public:
		using ProtobufMsgCallback = std::function<void(const NetChannelPtr&, const gpb::Message&)>;

		void Send(const NetChannelPtr& channel, const gpb::Message& msg) {
			m_Codec.Send(channel, msg);
		}
		void OnMessage(const NetChannelPtr& channel, const gpb::Message& msg) {
			m_MsgCallback(channel, msg);
		}

	private:
		T m_Codec;
		ProtobufMsgCallback m_MsgCallback;
	};

}