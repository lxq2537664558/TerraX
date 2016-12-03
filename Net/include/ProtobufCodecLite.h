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
/// | prependable bytes | prependable bytes| prependable bytes| prependable bytes|	prependable bytes|  readable bytes  |     CheckSum     |
/// |     totalLen      |  dest_peer_info  |   from_guestid   |   packetNameLen  |    packetName    |    (CONTENT)     | CRC32 or adler32 |
/// +-------------------+------------------+------------------+------------------+------------------+------------------+------------------+
/// |                   |                  |                  |                  |  (end with '\0') |                  |                  |
/// 0        <=       4Bytes     <=      4Bytes       <=    4Bytes       <=    1Bytes     <=     NameLen     <=    ConentSize    <=     4Bytes
/// @end

namespace TerraX
{
	namespace gpb =  google::protobuf;
	class ProtobufCodecLite
	{
		NOCOPY(ProtobufCodecLite);
		MAKEINSTANCE(ProtobufCodecLite);
	public:
		ProtobufCodecLite() = default;
		~ProtobufCodecLite() = default;
		const static int header_totallen_size = sizeof(int);
		const static int header_dest_size = sizeof(int);
		const static int header_from_size = sizeof(int);
		const static int header_pktlen_size = sizeof(uint8_t);
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
		void SendMsg(const NetChannelPtr& channel, const gpb::Message& msg, int32_t nFromGuestID, int32_t nDestPeerInfo)
		{
			struct evbuffer* buf = evbuffer_new();
			const std::string& msgName = msg.GetDescriptor()->name(); //if you want, you can use full_name() to replace it;
			const uint8_t name_len = static_cast<uint8_t>(msgName.size() + 1);

			const int header_size = header_totallen_size + header_dest_size + header_from_size + 
				header_pktlen_size + name_len;  
			const int content_size = msg.ByteSize();
			const int check_size = checksum_size;
			const int total_len = header_size + content_size + check_size;
			evbuffer_expand(buf, total_len);
			struct evbuffer_iovec vec;
			evbuffer_reserve_space(buf, total_len, &vec, 1);

			uint8_t* start = static_cast<uint8_t*>(vec.iov_base);
			int len_be = htonl(total_len);
			memcpy(start, &len_be, sizeof(len_be));
			start += header_totallen_size;
			int dest_be = htonl(nDestPeerInfo);
			memcpy(start, &dest_be, sizeof(dest_be));
			start += header_dest_size;
			int from_be = htonl(nFromGuestID);
			memcpy(start, &from_be, sizeof(from_be));
			start += header_from_size;
			memcpy(start, &name_len, sizeof(name_len));
			start += header_pktlen_size;
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
			start += checksum_size;
			assert(start - static_cast<uint8_t*>(vec.iov_base) == total_len);
			vec.iov_len = total_len;
			evbuffer_commit_space(buf, &vec, 1);
			channel->SendMsg(buf);
			evbuffer_free(buf);
		}
	
		void ReadMsg() {};

		uint32_t ExtractMsgDestInfo() { return 0; }
	};

}