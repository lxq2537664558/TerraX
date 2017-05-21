#include "dynamic_bitset.h"
using namespace terra;

dynamic_bitset::dynamic_bitset(std::size_t size) : size_(size)
{
    std::size_t array_size = size / kBitLength + 1;
    array_data_ = new uint32_t[array_size]();
}

dynamic_bitset::~dynamic_bitset() { delete[] array_data_; }

std::size_t dynamic_bitset::count() const
{
    std::size_t n = 0;
    for (std::size_t i = 0; i < array_size_; ++i) {
        n += count_bit(array_data_[i]);
    }
    return n;
}

bool dynamic_bitset::test(std::size_t pos) const
{
    assert(pos <= size_);
    return ((array_data_[pos / kBitLength] & ((uint32_t)1 << pos % kBitLength)) != 0);
}

bool dynamic_bitset::any() const
{
    for (std::size_t i = 0; i < array_size_; ++i) {
        if (array_data_[i] != 0) {
            return true;
        }
    }
    return false;
}

dynamic_bitset& dynamic_bitset::set()
{
    for (std::size_t i = 0; i < array_size_; ++i) {
        array_data_[i] = ~0;
    }
    return (*this);
}

dynamic_bitset& dynamic_bitset::set(std::size_t pos)
{
    assert(pos <= size_);
    array_data_[pos / kBitLength] |= (uint32_t)1 << pos % kBitLength;
    return (*this);
}

dynamic_bitset& dynamic_bitset::reset()
{
    for (std::size_t i = 0; i < array_size_; ++i) {
        array_data_[i] = 0;
    }
    return (*this);
}

dynamic_bitset& dynamic_bitset::reset(std::size_t pos)
{
    assert(pos <= size_);
    array_data_[pos / kBitLength] &= (uint32_t)0 << pos % kBitLength;
    return (*this);
}

dynamic_bitset& dynamic_bitset::flip()
{
    for (std::size_t i = 0; i < array_size_; ++i) {
        array_data_[i] = ~array_data_[i];
    }
    return (*this);
}
dynamic_bitset& dynamic_bitset::flip(std::size_t pos)
{
    assert(pos <= size_);
    array_data_[pos / kBitLength] ^= (uint32_t)1 << pos % kBitLength;
    return (*this);
}

std::string dynamic_bitset::to_string()
{
    std::string str;
    str.reserve(size_);
    for (std::size_t i = 0; i < size_; ++i) {
        if (test(i)) {
            str.append("1");
        } else {
            str.append("0");
        }
    }
    return str;
}

std::size_t dynamic_bitset::count_bit(uint32_t i) const
{
    i = i - ((i >> 1) & 0x55555555);
    i = (i & 0x33333333) + ((i >> 2) & 0x33333333);
    return (((i + (i >> 4)) & 0x0F0F0F0F) * 0x01010101) >> 24;
}