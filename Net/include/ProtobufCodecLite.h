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
/// |     totalLen      |  dest_peer_info  |  from_peerinfo   |   packetNameLen  |    packetName    |    (CONTENT)     | CRC32 or adler32 |
/// +-------------------+------------------+------------------+------------------+------------------+------------------+------------------+
/// |                   |                  |                  |                  |  (end with '\0') |                  |                  |
/// 0        <=       4Bytes     <=      4Bytes       <=    4Bytes       <=    1Bytes     <=     NameLen     <=    ConentSize    <=     4Bytes
/// @end

using namespace TerraX;
enum class ErrorCode_t
{
	eNoError,
	eInvalidLength,
	eCheckSumError,
	eParseError,
	eNoMoreData,
};

class MsgHeader_NoHeader
{
public:
	void parse(const char* data) {}
	void serialize(char* data) {}
	int32_t byte_size() { return 0; }
	int32_t dest_peer_info() { return 0; }
	int32_t from_peer_info() { return 0; }
	void set_dest_peer_info(int32_t dest_peer){}
	void set_from_peer_info(int32_t dest_peer) {}
};

class MsgHeader
{
public:
	MsgHeader() = default;
	MsgHeader(PeerType_t eDestPeerType){
		PeerInfo pi(eDestPeerType);
		m_dest_peer_info = pi.serialize();
	}
	//MsgHeader(int32_t m_)
	MsgHeader(int32_t dest_peer, int32_t from_peer) : 
		m_dest_peer_info(dest_peer), 
		m_from_peer_info(from_peer) {}
	void parse(const char* data) {
		memcpy(&m_dest_peer_info, data, sizeof(m_dest_peer_info));
		m_dest_peer_info = ntohl(m_dest_peer_info);
		memcpy(&m_from_peer_info, data + sizeof(m_dest_peer_info), sizeof(m_from_peer_info));
		m_from_peer_info = ntohl(m_from_peer_info);
	}

	void serialize(char* data) {
		m_dest_peer_info = htonl(m_dest_peer_info);
		memcpy(data, &m_dest_peer_info, sizeof(m_dest_peer_info));
		m_from_peer_info = htonl(m_from_peer_info);
		memcpy(data + sizeof(m_dest_peer_info), &m_from_peer_info, sizeof(m_from_peer_info));
	}
	int32_t byte_size() { return sizeof(m_dest_peer_info) + sizeof(m_from_peer_info); }
	int32_t dest_peer_info() { return m_dest_peer_info; }
	int32_t from_peer_info() { return m_from_peer_info; }
	void set_dest_peer_info(int32_t dest_peer) { m_dest_peer_info = dest_peer; }
	void set_from_peer_info(int32_t dest_peer) { m_from_peer_info = dest_peer; }
private:
	int32_t m_dest_peer_info{ 0 };
	int32_t m_from_peer_info{ 0 };
};

namespace gpb = google::protobuf;
class ProtobufCodecLite
{
	NOCOPY(ProtobufCodecLite);
	//MAKEINSTANCE(ProtobufCodecLite);
public:
	ProtobufCodecLite() = default;
	~ProtobufCodecLite() = default;

	template <class MsgHeaderT>
	void Serialize(evbuffer* buf, MsgHeaderT& msgHeader, const gpb::Message& msg);

	template <class MsgHeaderT>
	ErrorCode_t ReadMessage_OneByOne(struct evbuffer* input, struct evbuffer* output, MsgHeaderT& msgHeader);

	template <class MsgHeaderT>
	ErrorCode_t Parse(evbuffer* buf, MsgHeaderT& msgHeader, 
		char*& pMsgName, int32_t& nMsgNameSize, char*& pBuffer, int32_t& nBufferSize);

	template <class MsgHeaderT>
	void SetMsgHeader(evbuffer* buf, MsgHeaderT& msgHeader);

private:
	const static int HEADER_TOTALLEN_SIZE = sizeof(int);
	const static int HEADER_DEST_SIZE = sizeof(int);
	const static int HEADER_FROM_SIZE = sizeof(int);
	const static int HEADER_PKTLEN_SIZE = sizeof(uint8_t);
	const static int HEADER_PKTNAME_MINSIZE = sizeof('\0');
	const static int CHECKSUM_SIZE = sizeof(int);
	const static int MAX_MESSAGE_SIZE = 64 * 1024;
};

template <class MsgHeaderT>
void ProtobufCodecLite::Serialize(evbuffer* buf, MsgHeaderT& msgHeader, const gpb::Message& msg)
{
	const std::string& msgName = msg.GetDescriptor()->name(); //if you want, you can use full_name() to replace it;
	const uint8_t name_len = static_cast<uint8_t>(msgName.size() + 1);
	const int header_size = HEADER_TOTALLEN_SIZE + msgHeader.byte_size() + HEADER_PKTLEN_SIZE + name_len;
	const int content_size = msg.ByteSize();
	const int check_size = CHECKSUM_SIZE;
	const int total_len = header_size + content_size + check_size;
	evbuffer_expand(buf, total_len);
	struct evbuffer_iovec vec;
	evbuffer_reserve_space(buf, total_len, &vec, 1);
	char* start = static_cast<char*>(vec.iov_base);
	int len_be = htonl(total_len);
	memcpy(start, &len_be, sizeof(len_be));
	start += HEADER_TOTALLEN_SIZE;

	msgHeader.serialize(start);
	start += msgHeader.byte_size();

	memcpy(start, &name_len, sizeof(name_len));
	start += HEADER_PKTLEN_SIZE;
	memcpy(start, msgName.c_str(), name_len);
	start += name_len;
	bool res = msg.SerializeToArray(start, content_size);
	assert(res);
	start += content_size;
	// it does not contain [totalLen: 4bytes];
	int checkSum = static_cast<int>(::adler32(1, static_cast<const Bytef*>(vec.iov_base) + HEADER_TOTALLEN_SIZE,
		header_size + content_size - HEADER_TOTALLEN_SIZE));
	checkSum = htonl(checkSum);
	memcpy(start, &checkSum, sizeof(checkSum));
	start += CHECKSUM_SIZE;
	assert(start - static_cast<char*>(vec.iov_base) == total_len);
	vec.iov_len = total_len;
	evbuffer_commit_space(buf, &vec, 1);
}

template <class MsgHeaderT>
ErrorCode_t ProtobufCodecLite::ReadMessage_OneByOne(struct evbuffer* input, struct evbuffer* output, MsgHeaderT& msgHeader)
{
	std::size_t readable = evbuffer_get_length(input);
	int32_t min_msg_length = HEADER_TOTALLEN_SIZE + msgHeader.byte_size() + HEADER_PKTLEN_SIZE + HEADER_PKTNAME_MINSIZE + CHECKSUM_SIZE;
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
			evbuffer_remove_buffer(input, output, total_len);
			return ErrorCode_t::eNoError;
		}
		else
		{
			return ErrorCode_t::eNoMoreData;
		}
	}
	return ErrorCode_t::eNoMoreData;
}

template <class MsgHeaderT>
ErrorCode_t ProtobufCodecLite::Parse(evbuffer* buf, MsgHeaderT& msgHeader, char*& pMsgName, int32_t& nMsgNameSize, char*& pBuffer, int32_t& nBufferSize)
{
	ErrorCode_t errCode = ErrorCode_t::eNoError;
	int32_t be32 = 0;
	int32_t total_len = evbuffer_get_length(buf);
	char* data = reinterpret_cast<char*>(evbuffer_pullup(buf, total_len));
	memcpy(&be32, data + total_len - CHECKSUM_SIZE, sizeof(be32));
	int32_t expectedCheckSum = ntohl(be32);
	int32_t alldata_size = total_len - HEADER_TOTALLEN_SIZE - CHECKSUM_SIZE; // [totalLen] [checksum]
	int32_t checkSum = static_cast<int32_t>(
		::adler32(1, reinterpret_cast<const Bytef*>(data + HEADER_TOTALLEN_SIZE), alldata_size));
	int32_t start = 0;
	if (checkSum == expectedCheckSum)
	{
		start += HEADER_TOTALLEN_SIZE;
		//msg head info
		msgHeader.parse(data + start);
		start += msgHeader.byte_size();

		//get packetname_len:
		uint8_t packetname_len = 0;
		memcpy(&packetname_len, data + start, sizeof(packetname_len));
		start += HEADER_PKTLEN_SIZE;

		//get packet name:
		pMsgName = data + start;
		nMsgNameSize = packetname_len;
		start += packetname_len;

		//get message
		pBuffer = data + start;
		nBufferSize = total_len - start - CHECKSUM_SIZE;
		errCode = ErrorCode_t::eNoError;
	}
	else
	{
		errCode = ErrorCode_t::eCheckSumError;
	}

	return errCode;
}

template <class MsgHeaderT>
void ProtobufCodecLite::SetMsgHeader(evbuffer* buf, MsgHeaderT& msgHeader)
{
	int32_t nTotalLen = evbuffer_get_length(buf);
	assert(nTotalLen > HEADER_TOTALLEN_SIZE + msgHeader.byte_size() + HEADER_PKTLEN_SIZE + HEADER_PKTNAME_MINSIZE + CHECKSUM_SIZE);
	char* data = reinterpret_cast<char*>(evbuffer_pullup(buf, nTotalLen));
	msgHeader.serialize(data + HEADER_TOTALLEN_SIZE);
	int checkSum = static_cast<int>(::adler32(1, reinterpret_cast<const Bytef*>(data) + HEADER_TOTALLEN_SIZE,
		nTotalLen - HEADER_TOTALLEN_SIZE - CHECKSUM_SIZE));
	checkSum = htonl(checkSum);
	memcpy(data + nTotalLen - CHECKSUM_SIZE, &checkSum, sizeof(checkSum));
}