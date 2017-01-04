#pragma once
#include <zlib.h>
#include <mutex>
#include <queue>
#include <thread>
#include <google/protobuf/message.h>
#include "ComDef.h"

namespace gpb = google::protobuf;
namespace TerraX
{
	const int MAX_MESSAGE_SIZE = 64 * 1024;

    class Packet
    {
        DISABLE_COPY(Packet);

	public:
		Packet(std::size_t len);
		Packet(gpb::Message& msg);
		~Packet();
		// Tag: total_len-Destination-Owner
		void SetDestination(int dest_info);
		int GetDesination();
		void SetOwner(int owner_info);
		int GetOwnerInfo();

		bool IsValid();

		std::string GetPacketName();
		const char* GetPacketMsg();
		int GetMsgSize();


		char* GetBuffer() { return m_pBuffer; }
		std::size_t Size() { return m_Size; }
	public:
		static const int HEADER_SIZE = sizeof(int) + sizeof(int) + sizeof(int);
    private:
        char* m_pBuffer{nullptr};
        std::size_t m_Size{0};
    };

    class PacketQueue
    {
        DISABLE_COPY(PacketQueue);

    public:
        PacketQueue() = default;
        ~PacketQueue(){};

        void Push(Packet* pkt)
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            m_quePkt.push(std::unique_ptr<Packet>(std::move(pkt)));
        }
        Packet* Pop()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            Packet* p = (m_quePkt.front()).release();
            m_quePkt.pop();
            return p;
        }

        bool IsEmpty()
        {
            std::lock_guard<std::mutex> lock(m_mutex);
            return m_quePkt.empty();
        }

    private:
        std::queue<std::unique_ptr<Packet>> m_quePkt;
        std::mutex m_mutex;
    };
}