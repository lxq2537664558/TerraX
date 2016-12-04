#pragma once

#include "ComDef.h"
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <cassert>
#include <google/protobuf/message.h>
#include <zlib.h>
#include "NetChannel.h"


/// packet layout
/// @datagram
/// +-------------------+------------------+------------------+------------------+------------------+------------------+------------------+
/// | prependable bytes | prependable bytes| prependable bytes| prependable bytes|prependable bytes |  readable bytes  |     CheckSum     |
/// |     totalLen      |  dest_peer_info  |   from_guestid   |   packetNameLen  |    packetName    |    (CONTENT)     | CRC32 or adler32 |
/// +-------------------+------------------+------------------+------------------+------------------+------------------+------------------+
/// |                   |                  |                  |                  |  (end with '\0') |                  |                  |
/// 0        <=       4Bytes     <=      4Bytes       <=    4Bytes       <=    1Bytes     <=     NameLen     <=    ConentSize    <=     4Bytes
/// @end

namespace TerraX
{
	enum class ErrorCode_t
	{
		eNoError,
		eInvalidLength,
		eCheckSumError,
		eParseError,
		eNotDestination,
		eNoMoreData,
	};
	
	class MessagegInfo
	{
	public:
		MessagegInfo() = default;
		~MessagegInfo() = default;

	private:
		int32_t nTotalLen{ 0 };
		int32_t nDestPeerInfo{ 0 };
		int32_t nFromGuestID{ 0 };
		uint8_t nPacketLen{ 0 };
		char* pPacketData{ nullptr };
		int32_t nContentSize{ 0 };
		char* pContentData{ nullptr };
	};
	
	namespace gpb =  google::protobuf;
	class ProtobufCodecLite
	{
		NOCOPY(ProtobufCodecLite);
		MAKEINSTANCE(ProtobufCodecLite);
	public:
		ProtobufCodecLite() = default;
		~ProtobufCodecLite() = default;

		void SendMsg(const NetChannelPtr& channel, const gpb::Message& msg, int32_t nDestPeerInfo, int32_t nFromGuestID = 0)
		{
			struct evbuffer* buf = evbuffer_new();
			const std::string& msgName = msg.GetDescriptor()->name(); //if you want, you can use full_name() to replace it;
			const uint8_t name_len = static_cast<uint8_t>(msgName.size() + 1);

			const int header_size = HEADER_TOTALLEN_SIZE + HEADER_DEST_SIZE + HEADER_FROM_SIZE + 
				HEADER_PKTLEN_SIZE + name_len;  
			const int content_size = msg.ByteSize();
			const int check_size = CHECKSUM_SIZE;
			const int total_len = header_size + content_size + check_size;
			evbuffer_expand(buf, total_len);
			struct evbuffer_iovec vec;
			evbuffer_reserve_space(buf, total_len, &vec, 1);

			uint8_t* start = static_cast<uint8_t*>(vec.iov_base);
			int len_be = htonl(total_len);
			memcpy(start, &len_be, sizeof(len_be));
			start += HEADER_TOTALLEN_SIZE;
			int dest_be = htonl(nDestPeerInfo);
			memcpy(start, &dest_be, sizeof(dest_be));
			start += HEADER_DEST_SIZE;
			int from_be = htonl(nFromGuestID);
			memcpy(start, &from_be, sizeof(from_be));
			start += HEADER_FROM_SIZE;
			memcpy(start, &name_len, sizeof(name_len));
			start += HEADER_PKTLEN_SIZE;
			memcpy(start, msgName.c_str(), name_len);
			start += name_len;
			bool res = msg.SerializeToArray(start, content_size);
			assert(res);
			start += content_size;
			// it does not contain [totalLen: 4bytes];
			int checkSum = static_cast<int>(
				::adler32(1, static_cast<const Bytef*>(vec.iov_base) + 4, header_size + content_size - 4));
			checkSum = htonl(checkSum);
			memcpy(start, &checkSum, sizeof(checkSum));
			start += CHECKSUM_SIZE;
			assert(start - static_cast<uint8_t*>(vec.iov_base) == total_len);
			vec.iov_len = total_len;
			evbuffer_commit_space(buf, &vec, 1);
			channel->SendMsg(buf);
			evbuffer_free(buf);
		}
		
		bool IsDestination(int32_t peer_info, PeerType_t peer_type)
		{
			PeerInfo pi;
			pi.parse(peer_info);
			return pi.peer_type == peer_type;
		}

		ErrorCode_t parse(NetChannelPtr& channel, const char* buf, int total_len, PeerType_t peer_type) {
			ErrorCode_t error = ErrorCode_t::eNoError;

			// check sum
			int32_t be32 = 0;
			memcpy(&be32, buf + total_len - CHECKSUM_SIZE, sizeof(be32));
			int32_t expectedCheckSum = ntohl(be32);
			int32_t alldata_size = total_len - HEADER_TOTALLEN_SIZE - CHECKSUM_SIZE; // [totalLen] [checksum]
			int32_t checkSum = static_cast<int32_t>(
				::adler32(1, reinterpret_cast<const Bytef*>(buf + HEADER_TOTALLEN_SIZE), alldata_size));
			int32_t start = 0;
			if (checkSum == expectedCheckSum)
			{
				start += HEADER_TOTALLEN_SIZE;

				//dest info
				int32_t nDestPeerInfo = 0;
				memcpy(&nDestPeerInfo, buf + start, sizeof(nDestPeerInfo));
				nDestPeerInfo = ntohl(nDestPeerInfo);
				start += HEADER_DEST_SIZE;
				if (!IsDestination(nDestPeerInfo, peer_type))
				{
					return ErrorCode_t::eNotDestination;
				}

				//from guest info
				int32_t nFromGuestID = 0;
				memcpy(&nFromGuestID, buf + start, sizeof(nFromGuestID));
				nFromGuestID = ntohl(nFromGuestID);
				start += HEADER_FROM_SIZE;

				//get packetname_len:
				uint8_t packetname_len = 0;
				memcpy(&packetname_len, buf + start, sizeof(packetname_len));
				start += HEADER_PKTLEN_SIZE;

				//get packet name:
				std::string packetname(buf + start, packetname_len - 1); // '\0'
				start += packetname_len;

				//deliver packet
				assert(channel);
				if (channel->OnMessage(nFromGuestID, packetname, buf + start, total_len - start - 4)) {
				error = ErrorCode_t::eNoError;// or erro == kUnknownMessageType;
				}
				else {
				error = ErrorCode_t::eParseError;
				}
			}
			else
			{
				error = ErrorCode_t::eCheckSumError;
			}

			return error;
		}

		ErrorCode_t TryReadMsg(struct evbuffer* input, NetChannelPtr& channel, struct evbuffer* output, PeerType_t cur_peer) {
			std::size_t readable = evbuffer_get_length(input);
			int32_t min_msg_length = HEADER_TOTALLEN_SIZE + HEADER_DEST_SIZE +
				HEADER_FROM_SIZE + HEADER_PKTLEN_SIZE + 1 + CHECKSUM_SIZE;
			if (readable >= static_cast<std::size_t>(min_msg_length))
			{
				int be32 = 0;
				evbuffer_copyout(input, &be32, sizeof(be32));
				int total_len = ntohl(be32);
				if (total_len > MAX_MESSAGE_SIZE || total_len < min_msg_length)
				{
					return ErrorCode_t::eInvalidLength;
				}
				else if (readable >= static_cast<std::size_t>(total_len))
				{
					const char* data = reinterpret_cast<char*>(evbuffer_pullup(input, total_len));
					ErrorCode_t error = parse(channel, data, total_len, cur_peer);
					if (error == ErrorCode_t::eNotDestination)
					{
						evbuffer_remove_buffer(input, output, total_len);
						return error;
					}
					else
					{
						evbuffer_drain(input, total_len);
					}
					return error;
				}
				else
				{
					return ErrorCode_t::eNoMoreData;
				}
			}
			return ErrorCode_t::eNoMoreData;
		}


		void ReadAllMsg(struct evbuffer* input, NetChannelPtr& channel, struct evbuffer* output, PeerType_t cur_peer) {
			std::size_t readable = evbuffer_get_length(input);
			int32_t min_msg_length = HEADER_TOTALLEN_SIZE + HEADER_DEST_SIZE +
				HEADER_FROM_SIZE + HEADER_PKTLEN_SIZE + 1 + CHECKSUM_SIZE;
			while (readable >= static_cast<std::size_t>(min_msg_length))
			{
				int be32 = 0;
				evbuffer_copyout(input, &be32, sizeof(be32));
				int total_len = ntohl(be32);
				if (total_len > MAX_MESSAGE_SIZE)
				{
					channel->ForceClose(); // 数据错误，关闭掉该客户端;日志记录
					return;
				}
				if (total_len < min_msg_length)
				{
					break;
				}
				else if (readable >= static_cast<std::size_t>(total_len))
				{
					const char* data = reinterpret_cast<char*>(evbuffer_pullup(input, total_len));
					ErrorCode_t error = parse(channel, data, total_len, cur_peer);
					if (error == ErrorCode_t::eNotDestination)
					{
						evbuffer_remove_buffer(input, output, total_len);
					}
					else
					{
						evbuffer_drain(input, total_len);
					}
					readable = evbuffer_get_length(input);
				}
				else
				{
					break;
				}
			}
		}


		bool TryExtractDestInfo(struct evbuffer* buf, int32_t& nDestPeerInfo) {
			std::size_t readable = evbuffer_get_length(buf);
			int32_t min_msg_length = HEADER_TOTALLEN_SIZE + HEADER_DEST_SIZE +
				HEADER_FROM_SIZE + HEADER_PKTLEN_SIZE + 1 + CHECKSUM_SIZE;
			if (readable >= static_cast<std::size_t>(min_msg_length))
			{
				int be32 = 0;
				evbuffer_copyout(buf, &be32, sizeof(be32));
				int total_len = ntohl(be32);
				if (total_len > MAX_MESSAGE_SIZE || total_len < min_msg_length)
				{
					return false;
				}
				else if (readable >= static_cast<std::size_t>(total_len))
				{
					int32_t nDestStartIndex = HEADER_TOTALLEN_SIZE;
					const char* data = reinterpret_cast<char*>(evbuffer_pullup(buf, total_len));
					memcpy(&nDestPeerInfo, data + nDestStartIndex, sizeof(nDestPeerInfo));
					nDestPeerInfo = ntohl(nDestPeerInfo);
					return true;
				}
				else
				{
					return false;
				}
			}
			return false;
		}

		bool TrySetMsgInfo(struct evbuffer* buf, int32_t nDestPeerInfo, int32_t nFromGuestID) {
			std::size_t readable = evbuffer_get_length(buf);
			int32_t min_msg_length = HEADER_TOTALLEN_SIZE + HEADER_DEST_SIZE +
				HEADER_FROM_SIZE + HEADER_PKTLEN_SIZE + HEADER_PKTLEN_SIZE + CHECKSUM_SIZE;
			if (readable >= static_cast<std::size_t>(min_msg_length))
			{
				int be32 = 0;
				evbuffer_copyout(buf, &be32, sizeof(be32));
				int total_len = ntohl(be32);
				if (total_len > MAX_MESSAGE_SIZE || total_len < min_msg_length)
				{
					return false;
				}
				else if (readable >= static_cast<std::size_t>(total_len))
				{
					int32_t nDestStartIndex = HEADER_TOTALLEN_SIZE;
					int32_t nFromStartIndex = HEADER_TOTALLEN_SIZE + HEADER_DEST_SIZE;
					const char* data = reinterpret_cast<char*>(evbuffer_pullup(buf, total_len));
					nDestPeerInfo = htonl(nDestPeerInfo);
					memcpy((void*)(data + nDestStartIndex), &nDestPeerInfo, sizeof(nDestPeerInfo));
					nFromGuestID = htonl(nFromGuestID);
					memcpy((void*)(data + nFromStartIndex), &nFromGuestID, sizeof(nFromGuestID));
					return true;
				}
				else
				{
					return false;
				}
			}
			return false;
		}

	private:
		const static int HEADER_TOTALLEN_SIZE = sizeof(int);
		const static int HEADER_DEST_SIZE = sizeof(int);
		const static int HEADER_FROM_SIZE = sizeof(int);
		const static int HEADER_PKTLEN_SIZE = sizeof(uint8_t);
		const static int CHECKSUM_SIZE = sizeof(int);
		const static int MAX_MESSAGE_SIZE = 64 * 1024;
	};

}