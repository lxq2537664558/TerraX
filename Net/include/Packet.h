#pragma once
#include <google/protobuf/message.h>
#include <zlib.h>
#include <mutex>
#include <queue>
#include <thread>
#include "base_macro.h"
#include "TerraXConfig.h"

namespace gpb = google::protobuf;
namespace TerraX
{
    //| total_size:2 -- sizewithoutdest:2 -- owner_info:4 --
    //| msg_name_len:1 -- msg_name:msgname.size()+1 --
    //| msg_data:msg_size -- check_sum:4 -- destination...:4*n
    const int MAX_PACKET_SIZE = 65535;
    const int MAX_PACKET_TAG_SIZE =
        sizeof(uint16_t) + sizeof(uint16_t) + sizeof(int) + sizeof(uint8_t) + 0xFF + sizeof(int);
    const int MAX_APPENDDATA_SIZE = MAX_AOI_OBJ_COUNT * 4;
    const int MAX_MSG_SIZE = MAX_PACKET_SIZE - MAX_PACKET_TAG_SIZE - MAX_APPENDDATA_SIZE;

    class PacketBase
    {
        DISABLE_COPY(PacketBase);

    public:
        PacketBase(std::size_t len) : size_(len)
        {
            assert(len <= MAX_PACKET_SIZE);
            if (size_ > 0) {
                buffer_ = new char[len];
            }
        }
        virtual ~PacketBase() { delete[] buffer_; }
        std::size_t capacity() { return size_; }
        virtual char* buffer() { return buffer_; }
        virtual bool is_valid() { return true; }

    protected:
        char* buffer_{nullptr};
        std::size_t size_;
    };

    class PacketS : public PacketBase
    {
        DISABLE_COPY(PacketS);

    public:
        PacketS(std::size_t len);
        PacketS(gpb::Message& msg, int nReceiverCount = 1);
        ~PacketS() = default;
        bool AppendDestination(int dest_info);
        void GetAllDesination(int*& arrDest, int& nCount);
        void SetOwner(int owner_info);
        int GetOwnerInfo();

        bool is_valid() override final;

        std::string GetPacketName();
        const char* GetPacketMsg();
        int GetMsgSize();

        std::size_t GetPacketSize() { return m_PacketSize; }
        char* GetDataBuffer() { return buffer_ + sizeof(uint16_t); }
        std::size_t GetDataSize() { return m_DataSize; }

    public:
        static const int HEADER_SIZE = sizeof(uint16_t) + sizeof(uint16_t) + sizeof(int);

    private:
		std::size_t m_DataSize{0};
		std::size_t m_PacketSize{0};
    };

    class PacketC : public PacketBase
    {
        DISABLE_COPY(PacketC);

    public:
        PacketC(std::size_t len);
        PacketC(std::size_t data_size, std::size_t len);
        PacketC(gpb::Message& msg);
        int GetCltDestionation();
        void SetOwner(int owner_info);
        int GetOwnerInfo();
        void AppendDestination(int dest_info);

        std::string GetPacketName();
        const char* GetPacketMsg();
        int GetMsgSize();
        char* buffer() override final { return buffer_ + off_set_; }
        bool is_valid() override final;

    public:
        static const int EX_DATA_SIZE = sizeof(uint16_t) + sizeof(int);
        static const int HEADER_SIZE = sizeof(uint16_t) + sizeof(int);

    private:
        const int off_set_{0};
        std::size_t m_DataSize{0};
    };
   
    class PacketQueue
    {
        DISABLE_COPY(PacketQueue);

    public:
        PacketQueue() = default;
        ~PacketQueue(){};

        void Push(PacketBase* pkt)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_quePkt.push(std::unique_ptr<PacketBase>(pkt));
        }
        PacketBase* Pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            PacketBase* p = (m_quePkt.front()).release();
            m_quePkt.pop();
            return p;
        }

        bool IsEmpty()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_quePkt.empty();
        }

    private:
        std::queue<std::unique_ptr<PacketBase>> m_quePkt;
        std::mutex m_mutex;
    };
}
