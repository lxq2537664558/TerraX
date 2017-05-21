#pragma once

#include <google/protobuf/message.h>
#include "base/types.h"
#include <arpa/inet.h>

namespace terra
{
    const int MAX_PACKET_SIZE = 64000;  // TCP MSS = 65535B-40B
    const int PACKET_BUFFER_SIZE = 4000;
    class MsgTag
    {
	public:
		static const int TAG_SIZE = sizeof(int);
    private:
        char* msg_tag_{nullptr};
        int server_id_{-1};

    public:
        MsgTag() = default;
        ~MsgTag() = default;

        bool InitialWithMsg(char* buffer);
        bool InitialFromBuffer(char* buffer);
        void set_server_id(int server_id)
        {
            assert(msg_tag_);
            server_id_ = server_id;
            int be32 = htonl(server_id);
            memcpy(msg_tag_, &be32, sizeof be32);
        }
        int get_tag_size() { return TAG_SIZE; }
    };

    class MsgData
    {
	public:
		static const int MIN_MSG_DATA_SIZE = sizeof(uint16_t) + sizeof(int) + 2;
    private:
		// msg_data_size_ + avatar_id_ + 1('\0') + 1
        char* msg_data_{nullptr};
        uint16_t msg_data_size_{0};
		int avatar_id_{ 0 };
		char* msg_name_{ nullptr };
		uint8_t msg_name_size_{ 0 };
		char* msg_{ nullptr };
		int msg_size_{ 0 };

    public:
        MsgData() = default;
        ~MsgData() = default;

        bool InitialWithMsg(char* buffer, int size, google::protobuf::Message& msg);
        bool InitialFromBuffer(char* buffer);

        void set_avatar_id(int avatar_id)
        {
            assert(msg_data_);
            avatar_id_ = avatar_id;
            int be32 = htonl(avatar_id);
            memcpy(msg_data_ + sizeof(uint16_t), &be32, sizeof be32);
        }
        int get_avatar_id() const { return avatar_id_; }

        const char* get_msg_data() { return msg_data_; }
        int get_msg_data_size() { return msg_data_size_; }

        const char* get_msg_name() { return msg_name_; }
        const char* get_msg() { return msg_; }
        int get_msg_size() { return msg_size_; }
    };

    class Packet
    {
	public:
		static const int MIN_PACKET_SIZE = sizeof(uint16_t) + MsgData::MIN_MSG_DATA_SIZE;
		static const int MIN_PACKET_SIZE_WITH_TAG = MIN_PACKET_SIZE + MsgTag::TAG_SIZE;
    private:
		//bool use_alloc_buffer{false};
		//buffer = smallbufer;
        char small_buffer_[4096]{0};
        // char* buffer_{ nullptr };
        uint16_t total_len_{0};
        MsgTag msg_tag_;
        MsgData msg_data_;
        uint16_t avatar_count_{0};
        uint16_t max_avatar_count_{0};
        int* avatars_{nullptr};

    public:
        Packet() = default;
        ~Packet() { /*delete[] big_buffer_;*/}
        void InitialWithMsg(google::protobuf::Message& msg, int max_avatar_count = 0);
        void InitialFromBuffer(uint16_t total_len);

        char* get_buffer() { return small_buffer_; }
        uint16_t get_total_len() { return total_len_; }

        void set_avatar_id(int avatar_id) { msg_data_.set_avatar_id(avatar_id); }
        void set_server_id(int server_id) { msg_tag_.set_server_id(server_id); }

        MsgTag& get_msg_tag() { return msg_tag_; }
        MsgData& get_msg_data() { return msg_data_; }

    private:
        void set_total_len(uint16_t len)
        {
            assert(len);
            total_len_ = len;
			uint16_t be16 = htons(len);
            memcpy(small_buffer_, &be16, sizeof be16);
        }
    };
}