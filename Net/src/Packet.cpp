#include "Packet.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif
using namespace TerraX;

Packet::Packet(std::size_t len) : m_Size(len)
{
    m_pBuffer = new char[m_Size]{0};
}

Packet::Packet(gpb::Message& msg)
{
    // if you want, you can use full_name() to replace it;
    const std::string& msgName = msg.GetDescriptor()->name();
    const uint8_t name_len = static_cast<uint8_t>(msgName.size() + 1);  //'\0'
    //[total_len-Destination-Owner] + sizeof(name_len) + namelen +
    // msg.byte_size + [sizeof(checksum)]
    const int total_data_size = sizeof(name_len) + name_len + msg.ByteSize();

    m_Size = HEADER_SIZE + total_data_size + sizeof(int);
    m_pBuffer = new char[m_Size]{0};

    int len_be = htonl(m_Size);
    memcpy(m_pBuffer, &len_be, sizeof len_be);

    int start_index = HEADER_SIZE;
    memcpy(m_pBuffer + start_index, &name_len, sizeof name_len);
    start_index += sizeof name_len;

    memcpy(m_pBuffer + start_index, msgName.c_str(), name_len);
    start_index += name_len;

    msg.SerializeToArray(m_pBuffer + start_index, msg.ByteSize());
    start_index += msg.ByteSize();

    int checkSum = static_cast<int>(
        ::adler32(1, reinterpret_cast<const Bytef*>(m_pBuffer) + HEADER_SIZE,
                  total_data_size));
    checkSum = htonl(checkSum);
    memcpy(m_pBuffer + start_index, &checkSum, sizeof(checkSum));
}

Packet::~Packet() { delete[] m_pBuffer; }

std::string Packet::GetPacketName()
{
	uint8_t name_len = 0;
	memcpy(&name_len, m_pBuffer + HEADER_SIZE, sizeof name_len);
	char* pMsgName = m_pBuffer + HEADER_SIZE + sizeof(uint8_t);
	return std::move(std::string(pMsgName, name_len - 1));
}

const char* Packet::GetPacketMsg()
{
	uint8_t name_len = 0;
	memcpy(&name_len, m_pBuffer + HEADER_SIZE, sizeof name_len);
	char* pMsg = m_pBuffer + HEADER_SIZE + sizeof(uint8_t) + name_len;
	return pMsg;
}
int Packet::GetMsgSize()
{
	uint8_t name_len = 0;
	memcpy(&name_len, m_pBuffer + HEADER_SIZE, sizeof name_len);
	int nMsgSize = m_Size - HEADER_SIZE - sizeof(uint8_t) - name_len - sizeof(int);
	return nMsgSize;
}

bool Packet::IsValid()
{
	int32_t expectedCheckSum = static_cast<int>(
		::adler32(1, reinterpret_cast<const Bytef*>(m_pBuffer) + HEADER_SIZE,
			m_Size - HEADER_SIZE - sizeof(int)));
	int checkSum = 0;
	memcpy(&checkSum, m_pBuffer + m_Size - sizeof(int), sizeof(checkSum));
	checkSum = ntohl(checkSum);
	return checkSum == expectedCheckSum;
}


void Packet::SetDestination(int dest_info)
{
	dest_info = htonl(dest_info);
	memcpy(m_pBuffer + sizeof(int), &dest_info, sizeof dest_info);
}
int Packet::GetDesination()
{
	int dest_info = 0;
	memcpy(&dest_info, m_pBuffer + sizeof(int), sizeof dest_info);
	return ntohl(dest_info);
}
void Packet::SetOwner(int owner_info)
{
	owner_info = htonl(owner_info);
	memcpy(m_pBuffer + sizeof(int) + sizeof(int), &owner_info, sizeof owner_info);
}
int Packet::GetOwnerInfo()
{
	int owner_info = 0;
	memcpy(&owner_info, m_pBuffer + sizeof(int) + sizeof(int), sizeof owner_info);
	return ntohl(owner_info);
}