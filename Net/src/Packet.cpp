#include "Packet.h"

#ifdef _WIN32
#include <WinSock2.h>
#else
#include <arpa/inet.h>
#endif
using namespace TerraX;

PacketS::PacketS(std::size_t len) : PacketBase(len) { m_PacketSize = len; }

PacketS::PacketS(gpb::Message& msg, int nReceiverCount /* = 1*/) : PacketBase(0)
{
    assert(msg.ByteSize() <= MAX_MSG_SIZE);
    // if you want, you can use full_name() to replace it;
    const std::string& msgName = msg.GetDescriptor()->name();
    assert(msgName.size() + 1 <= 0xFF);
    const uint8_t name_len = static_cast<uint8_t>(msgName.size() + 1);  //'\0'
    const int total_data_size = sizeof(name_len) + name_len + msg.ByteSize();

    m_PacketSize = HEADER_SIZE + total_data_size + sizeof(int);
    m_DataSize = m_PacketSize - sizeof(uint16_t);
    size_ = sizeof(uint16_t) + m_DataSize + sizeof(int) * nReceiverCount;
    buffer_ = new char[size_];
    int start_index = 0;
    uint16_t packet_be16 = htons(static_cast<uint16_t>(m_PacketSize));
    memcpy(buffer_, &packet_be16, sizeof packet_be16);
    start_index += sizeof packet_be16;

    uint16_t data_be16 = htons(static_cast<uint16_t>(m_DataSize));
    memcpy(buffer_ + start_index, &data_be16, sizeof data_be16);
    start_index += sizeof data_be16;

    start_index += sizeof(int32_t);

    memcpy(buffer_ + start_index, &name_len, sizeof name_len);
    start_index += sizeof name_len;

    memcpy(buffer_ + start_index, msgName.c_str(), name_len);
    start_index += name_len;

    msg.SerializeToArray(buffer_ + start_index, msg.ByteSize());
    start_index += msg.ByteSize();

    int checkSum = static_cast<int>(
        ::adler32(1, reinterpret_cast<const Bytef*>(buffer_) + HEADER_SIZE, total_data_size));
    checkSum = htonl(checkSum);
    memcpy(buffer_ + start_index, &checkSum, sizeof(checkSum));
}

std::string PacketS::GetPacketName()
{
    uint8_t name_len = 0;
    memcpy(&name_len, buffer_ + HEADER_SIZE, sizeof name_len);
    char* pMsgName = buffer_ + HEADER_SIZE + sizeof(uint8_t);
    return std::move(std::string(pMsgName, name_len - 1));
}

const char* PacketS::GetPacketMsg()
{
    uint8_t name_len = 0;
    memcpy(&name_len, buffer_ + HEADER_SIZE, sizeof name_len);
    char* pMsg = buffer_ + HEADER_SIZE + sizeof(uint8_t) + name_len;
    return pMsg;
}
int PacketS::GetMsgSize()
{
    uint8_t name_len = 0;
    memcpy(&name_len, buffer_ + HEADER_SIZE, sizeof name_len);
    int nMsgSize = m_DataSize + sizeof(uint16_t) - HEADER_SIZE - sizeof(uint8_t) - name_len - sizeof(int);
    return nMsgSize;
}

bool PacketS::is_valid()
{
    uint16_t data_size = 0;
    memcpy(&data_size, buffer_ + sizeof(data_size), sizeof(data_size));
    m_DataSize = ntohs(data_size);
    int32_t expectedCheckSum =
        static_cast<int>(::adler32(1, reinterpret_cast<const Bytef*>(buffer_) + HEADER_SIZE,
                                   m_DataSize + sizeof(uint16_t) - HEADER_SIZE - sizeof(int)));
    int checkSum = 0;
    memcpy(&checkSum, buffer_ + m_DataSize + sizeof(uint16_t) - sizeof(int), sizeof(checkSum));
    checkSum = ntohl(checkSum);
    return checkSum == expectedCheckSum;
}

bool PacketS::AppendDestination(int dest_info)
{
    if ((m_PacketSize + sizeof(dest_info)) > size_) {
        assert(0);
        return false;
    }
    memcpy(buffer_ + m_PacketSize, &dest_info, sizeof dest_info);
    m_PacketSize += sizeof(dest_info);
    uint16_t packet_be16 = htons(static_cast<uint16_t>(m_PacketSize));
    memcpy(buffer_, &packet_be16, sizeof packet_be16);
    return true;
}

void PacketS::GetAllDesination(int*& arrDest, int& nCount)
{
    assert(m_DataSize > 0);
    nCount = (m_PacketSize - m_DataSize - sizeof(uint16_t)) / sizeof(int);
    arrDest = reinterpret_cast<int*>(buffer_ + m_DataSize + sizeof(uint16_t));
}

void PacketS::SetOwner(int owner_info)
{
    owner_info = htonl(owner_info);
    memcpy(buffer_ + sizeof(uint16_t) + sizeof(uint16_t), &owner_info, sizeof owner_info);
}
int PacketS::GetOwnerInfo()
{
    int owner_info = 0;
    memcpy(&owner_info, buffer_ + +sizeof(uint16_t) + sizeof(uint16_t), sizeof owner_info);
    return ntohl(owner_info);
}
//////////////////////////////////////////////////////////////////////////
PacketC::PacketC(std::size_t len) : PacketBase(len) { m_DataSize = len; }

PacketC::PacketC(std::size_t data_size, std::size_t ex_data_size)
    : off_set_(sizeof(uint16_t)), PacketBase(data_size + ex_data_size)
{
    m_DataSize = data_size;
    uint16_t len_be16 = htons(static_cast<uint16_t>(size_));
    memcpy(buffer_, &len_be16, sizeof(len_be16));
}
PacketC::PacketC(gpb::Message& msg) : PacketBase(0)
{
    assert(msg.ByteSize() <= MAX_MSG_SIZE);
    // if you want, you can use full_name() to replace it;
    const std::string& msgName = msg.GetDescriptor()->name();
    assert(msgName.size() + 1 <= 0xFF);
    const uint8_t name_len = static_cast<uint8_t>(msgName.size() + 1);  //'\0'
    const int total_data_size = sizeof(name_len) + name_len + msg.ByteSize();

    size_ = m_DataSize = HEADER_SIZE + total_data_size + sizeof(int);
    buffer_ = new char[size_];

    int start_index = 0;
    uint16_t len_be16 = htons(static_cast<uint16_t>(size_));
    memcpy(buffer_, &len_be16, sizeof len_be16);
    start_index += sizeof len_be16;

    start_index += sizeof(int32_t);

    memcpy(buffer_ + start_index, &name_len, sizeof name_len);
    start_index += sizeof name_len;

    memcpy(buffer_ + start_index, msgName.c_str(), name_len);
    start_index += name_len;

    msg.SerializeToArray(buffer_ + start_index, msg.ByteSize());
    start_index += msg.ByteSize();

    int checkSum = static_cast<int>(
        ::adler32(1, reinterpret_cast<const Bytef*>(buffer_) + HEADER_SIZE, total_data_size));
    checkSum = htonl(checkSum);
    memcpy(buffer_ + start_index, &checkSum, sizeof(checkSum));
}
int PacketC::GetCltDestionation()
{
    int cltdest_info = 0;
    memcpy(&cltdest_info, buffer_ + off_set_ + sizeof(uint16_t), sizeof cltdest_info);
    return ntohl(cltdest_info);
}
int PacketC::GetOwnerInfo()
{
    int cltdest_info = 0;
    memcpy(&cltdest_info, buffer_ + off_set_ + sizeof(uint16_t), sizeof cltdest_info);
    return ntohl(cltdest_info);
}
void PacketC::SetOwner(int owner_info)
{
    owner_info = htonl(owner_info);
    memcpy(buffer_ + off_set_ + sizeof(uint16_t), &owner_info, sizeof owner_info);
}
void PacketC::AppendDestination(int dest_info)
{
    if (m_DataSize == size_) {
        return;
    }
    memcpy(buffer_ + size_ - sizeof(int), &dest_info, sizeof dest_info);
}
std::string PacketC::GetPacketName()
{
    uint8_t name_len = 0;
    memcpy(&name_len, buffer_ + HEADER_SIZE + off_set_, sizeof name_len);
    char* pMsgName = buffer_ + HEADER_SIZE + off_set_ + sizeof(uint8_t);
    return std::move(std::string(pMsgName, name_len - 1));
}

const char* PacketC::GetPacketMsg()
{
    uint8_t name_len = 0;
    memcpy(&name_len, buffer_ + HEADER_SIZE + off_set_, sizeof name_len);
    char* pMsg = buffer_ + HEADER_SIZE + off_set_ + sizeof(uint8_t) + name_len;
    return pMsg;
}
int PacketC::GetMsgSize()
{
    uint8_t name_len = 0;
    memcpy(&name_len, buffer_ + HEADER_SIZE + off_set_, sizeof name_len);
    int nMsgSize = m_DataSize - HEADER_SIZE - sizeof(uint8_t) - name_len - sizeof(int);
    return nMsgSize;
}
bool PacketC::is_valid()
{
    int32_t expectedCheckSum =
        static_cast<int>(::adler32(1, reinterpret_cast<const Bytef*>(buffer_) + HEADER_SIZE + off_set_,
                                   m_DataSize - HEADER_SIZE - sizeof(int)));
    int checkSum = 0;
    memcpy(&checkSum, buffer_ + m_DataSize + off_set_ - sizeof(int), sizeof(checkSum));
    checkSum = ntohl(checkSum);
    return checkSum == expectedCheckSum;
}
//////////////////////////////////////////////////////////////////////////

