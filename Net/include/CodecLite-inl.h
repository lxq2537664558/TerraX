#pragma once
#include <cstdint>
#include <event2/bufferevent.h>
#include <event2/buffer.h>
#include <cassert>
#include <google/protobuf/message.h>
#include <zlib.h>
#include "NetChannel.h"

namespace TerraX
{
	/// packet layout
	/// @datagram
	/// +-------------------+------------------+------------------+------------------+------------------+------------------+
	/// | prependable bytes | prependable bytes| prependable bytes|	prependable bytes|  readable bytes  |     CheckSum     |
	/// |     totalLen      |       flag       |   packetNameLen  |    packetName    |    (CONTENT)     | CRC32 or adler32 |
	/// +-------------------+------------------+------------------+------------------+------------------+------------------+
	/// |                   |                  |                  |  (end with '\0') |                  |                  |
	/// 0        <=       4Bytes     <=      4Bytes       <=    2Bytes     <=     NameLen     <=    ConentSize    <=     4Bytes
	/// @end
	inline void send(struct bufferevent* bev, int flag, const google::protobuf::Message& message)
	{
		struct evbuffer* buf = evbuffer_new();
		const std::string& msgName = message.GetDescriptor()->name(); //if you want, you can use full_name() to replace it;
		const uint16_t name_len = static_cast<uint16_t>(msgName.size() + 1);

		const int header_size = 4 + 4 + 2 + name_len; // total_len + flag + packet_name_len + name_len; 
		const int content_size = message.ByteSize();
		const int check_size = 4;
		const int total_len = header_size + content_size + check_size;
		evbuffer_expand(buf, total_len);
		struct evbuffer_iovec vec;
		evbuffer_reserve_space(buf, total_len, &vec, 1);

		uint8_t* start = static_cast<uint8_t*>(vec.iov_base);
		int len_be = htonl(total_len);
		memcpy( start, &len_be, sizeof(len_be) );
		start += 4;
		int flag_be = htonl(flag);
		memcpy( start, &flag_be, sizeof(flag_be) );
		start += 4;
		uint16_t nameLen_be = htons(name_len);
		memcpy(start, &nameLen_be, sizeof(nameLen_be));
		start += 2;
		memcpy(start, msgName.c_str(), name_len);
		start += name_len;
		bool res = message.SerializeToArray(start, content_size);
		assert(res);
		start += content_size;
		// it does not contain [totalLen: 4bytes];
		int checkSum = static_cast<int>(
			::adler32(1, static_cast<const Bytef*>(vec.iov_base) + 4, header_size + content_size - 4));
		checkSum = htonl(checkSum);
		memcpy(start, &checkSum, sizeof(checkSum));
		start += 4;
		assert(start - static_cast<uint8_t*>(vec.iov_base) == total_len);
		vec.iov_len = total_len;
		evbuffer_commit_space(buf, &vec, 1);
		bufferevent_write_buffer(bev, buf);
		evbuffer_free(buf);
	}

	enum ParseErrorCode
	{
		kNoError = 0,
		kInvalidLength,
		kCheckSumError,
		kInvalidNameLen,
		kUnknownMessageType,
		kParseError,
	};

	inline ParseErrorCode parse(NetChannel* channel, const char* buf, int total_len)
	{
		ParseErrorCode error = kNoError;

		// check sum
		int32_t be32 = 0;
		memcpy(&be32, buf + total_len - 4, sizeof(be32) );
		int32_t expectedCheckSum = ntohl(be32);
		int32_t alldata_size = total_len - 4 - 4; // [totalLen] [checksum]
		int32_t checkSum = static_cast<int32_t>(
			::adler32(1, reinterpret_cast<const Bytef*>(buf + 4), alldata_size));
		int32_t start = 0;
		if (checkSum == expectedCheckSum)
		{
			start += 4;
			//get flag:
			int32_t flag = 0;
			memcpy(&flag, buf + start, sizeof(flag));
			flag = ntohl(flag);
			start += 4;
			//do check flag?

			//get packetname_len:
			uint16_t packetname_len = 0;
			memcpy(&packetname_len, buf + start, sizeof(packetname_len));
			packetname_len = ntohs(packetname_len);
			start += 2;
			//get packet name:
			std::string packetname(buf + start, packetname_len - 1); // '\0'
			start += packetname_len;

			//deliver packet
			assert(channel);
			if (channel->onMessage(packetname, buf + start, total_len - start - 4)) {
				error = kNoError;// or erro == kUnknownMessageType;
			}
			else {
				error = kParseError; 
			}
		}
		else
		{
			error = kCheckSumError;
		}

		return error;
	}

	inline ParseErrorCode read(struct evbuffer* input, NetChannel* channel)
	{
		ParseErrorCode error = kNoError;
		std::size_t readable = evbuffer_get_length(input);
		while (readable >= 12)
		{
			int be32 = 0;
			evbuffer_copyout(input, &be32, sizeof(be32) );
			int total_len = ntohl(be32);
			if (total_len > 64 * 1024 * 1024 || total_len < 4 + 4 + 2 + 1 + 4)
			{
				error = kInvalidLength;
				break;
			}
			else if (readable >= static_cast<std::size_t>(total_len))
			{
				const char* data = reinterpret_cast<char*>(evbuffer_pullup(input, total_len));
				error = parse(channel, data, total_len);
				if (error != kNoError)
				{
					// maybe we should record it why return error;
				}
				evbuffer_drain(input, total_len);
				readable = evbuffer_get_length(input);
			}
			else
			{
				break;
			}
		}
		return error;
	}
}