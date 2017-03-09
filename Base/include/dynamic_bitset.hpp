#pragma once
#include <cassert>
#include <iostream>
namespace TerraX
{
	class dynamic_bitset
	{
	private:
		std::size_t size_;
		std::size_t arrLength_;
		uint32_t* arrNumbers_;
		static const int BIT_LENGTH = sizeof(std::size_t) * 8;

	public:
		explicit dynamic_bitset(std::size_t size) : size_(size)
		{
			std::size_t arrLength_ = size / BIT_LENGTH + 1;
			arrNumbers_ = new uint32_t[arrLength_]{ 0 };
		}
		~dynamic_bitset() { delete[] arrNumbers_; }

		std::size_t count() const
		{
			std::size_t n = 0;
			for (std::size_t i = 0; i < arrLength_; ++i) {
				n += count_bit(arrNumbers_[i]);
			}
			return n;
		}
		std::size_t size() const { return size_; }
		bool test(std::size_t pos) const
		{
			assert(pos <= size_);
			return ((arrNumbers_[pos / BIT_LENGTH] & ((uint32_t)1 << pos % BIT_LENGTH)) != 0);
		}
		bool any() const
		{
			for (std::size_t i = 0; i < arrLength_; ++i) {
				if (arrNumbers_[i] != 0) {
					return true;
				}
			}
			return false;
		}
		bool none() const { return (!any()); }
		bool all() const { return (count() == size()); }

		dynamic_bitset& set()
		{
			for (std::size_t i = 0; i < arrLength_; ++i) {
				arrNumbers_[i] = ~0;
			}
			return (*this);
		}
		dynamic_bitset& set(std::size_t pos)
		{
			assert(pos <= size_);
			arrNumbers_[pos / BIT_LENGTH] |= (uint32_t)1 << pos % BIT_LENGTH;
			return (*this);
		}

		dynamic_bitset& reset()
		{
			for (std::size_t i = 0; i < arrLength_; ++i) {
				arrNumbers_[i] = 0;
			}
			return (*this);
		}
		dynamic_bitset& reset(std::size_t pos)
		{
			assert(pos <= size_);
			arrNumbers_[pos / BIT_LENGTH] &= (uint32_t)0 << pos % BIT_LENGTH;
			return (*this);
		}

		dynamic_bitset& flip()
		{
			for (std::size_t i = 0; i < arrLength_; ++i) {
				arrNumbers_[i] = ~arrNumbers_[i];
			}
			return (*this);
		}
		dynamic_bitset& flip(std::size_t pos)
		{
			assert(pos <= size_);
			arrNumbers_[pos / BIT_LENGTH] ^= (uint32_t)1 << pos % BIT_LENGTH;
			return (*this);
		}
		std::string to_string()
		{
			std::string str;
			str.reserve(size_);
			for (std::size_t i = 0; i < size_; ++i) {
				if (test(i)) {
					str.append("1");
				}
				else {
					str.append("0");
				}
			}
			return str;
		}

	private:
		std::size_t count_bit(uint32_t i) const
		{
			i = i - ((i >> 1) & 0x55555555);
			i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
			return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
		}
	};
}