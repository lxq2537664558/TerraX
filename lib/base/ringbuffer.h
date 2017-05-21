#pragma once

#include "types.h"

namespace terra
{
	//brevity is the soul of wit.
	class ring_buffer
	{
	private:
		int in_{ 0 };
		int out_{ 0 };
		int size_{ 0 };
		char* buffer_{ nullptr };
	public:
		ring_buffer(int size) : size_(size)
		{
			assert(size_ > 0 && ((size_ & (size_ - 1)) == 0));
			buffer_ = new char[size_];
		}
		~ring_buffer() { delete[] buffer_; }

		void write(const char* data, int len)
		{
			assert(writable_size() >= len);
			len = MIN(len, size_ - in_ + out_);

			auto l = MIN(len, size_ - (in_ & (size_ - 1)));
			memcpy(buffer_ + (in_ & (size_ - 1)), data, l);
			memcpy(buffer_, data + l, len - l);

			in_ += len;
		}
		void read(char* data, int len)
		{
			assert(readable_size() >= len);
			len = MIN(len, in_ - out_);

			auto l = MIN(len, size_ - (out_ & (size_ - 1)));
			memcpy(data, buffer_ + (out_ & (size_ - 1)), l);
			memcpy(data + l, buffer_, len - l);

			out_ += len;
		}
		void peek(char* data, int len)
		{
			assert(readable_size() >= len);
			len = MIN(len, in_ - out_);

			auto l = MIN(len, size_ - (out_ & (size_ - 1)));
			memcpy(data, buffer_ + (out_ & (size_ - 1)), l);
			memcpy(data + l, buffer_, len - l);
		}

		int readable_size() { return in_ - out_; }
		int writable_size() { return size_ - in_ + out_; }
		int buffer_size() { return size_; }
	};
}