#pragma once

#include <uuid/uuid.h>
#include "types.h"

namespace terra
{
    class GuidX
    {
    private:
        union Guid_t {
            struct GUID_1 {
                unsigned long data1;
                unsigned short data2;
                unsigned short data3;
                unsigned char data4[8];
            } guid1;
            struct GUID_2 {
                uint64_t data1;
                uint64_t data2;
            } guid2;
            char guid[sizeof guid1];
        };

        Guid_t guid_;

    public:
        GuidX() { memset(&guid_, 0, sizeof guid_); }
        GuidX() = default;

        static GuidX Generate()
        {
            GuidX guid;
            uuid_t uuid;
            uuid_generate(&uuid[0]);
            memcpy(&guid.guid_, &uuid, sizeof uuid);
            return guid;
        }

        GuidX& operator=(const GuidX& rhs)
        {
            guid_.guid2.data1 = rhs.guid_.guid2.data1;
            guid_.guid2.data2 = rhs.guid_.guid2.data2;
            return *this;
        }

        static_assert(sizeof(Guid_t::GUID_1) == sizeof(Guid_t::GUID_2), "error guid type!");
    };
}