#pragma once

#include "types.h"

namespace terra
{
    class dynamic_bitset
    {
    private:
        std::size_t size_;
        std::size_t array_size_;
        uint32_t* array_data_;
        static const int kBitLength = sizeof(std::size_t) * 8;

    public:
        explicit dynamic_bitset(std::size_t size);
        ~dynamic_bitset();

        std::size_t count() const;
        std::size_t size() const { return size_; }
        bool test(std::size_t pos) const;
		bool any() const;

        bool none() const { return (!any()); }
        bool all() const { return (count() == size()); }

        dynamic_bitset& set();
        dynamic_bitset& set(std::size_t pos);

        dynamic_bitset& reset();
        dynamic_bitset& reset(std::size_t pos);

        dynamic_bitset& flip();
        dynamic_bitset& flip(std::size_t pos);

        std::string to_string();

    private:
        std::size_t count_bit(uint32_t i) const;
    };
}