#pragma once

#include <cstdint>
#ifdef _WIN32
#include <combaseapi.h>
#else
#include <uuid/uuid.h>
#endif  // _WIN32

namespace TerraX
{
    class TerraXGuid
    {
    private:
        union XGuid {
            struct GUID_1 {
                unsigned long Data1;
                unsigned short Data2;
                unsigned short Data3;
                unsigned char Data4[8];
            } stGuid1;
            struct GUID_2 {
                uint64_t Data1;
                uint64_t Data2;
            } stGuid2;
            char szGuid[sizeof GUID_1];
        };
        XGuid m_XGuid;

    public:
        TerraXGuid() { memset(&m_XGuid, 0, sizeof(m_XGuid)); }
        ~TerraXGuid() = default;

        static TerraXGuid Generate()
        {
			TerraXGuid guid;
#ifdef _WIN32
            GUID _guid = GUID_NULL;
            ::CoCreateGuid(&_guid);
            memcpy(&guid.m_XGuid, &_guid, sizeof(GUID));
#else
            uuid_t uuid;
            uuid_generate(&uuid[0]);
            memcpy(&guid.m_XGuid, &uuid, sizeof(uuid_t));
#endif  // _WIN32
            return guid;
        }

		TerraXGuid& operator= (const TerraXGuid& rhs)
		{
			m_XGuid.stGuid2.Data1 = rhs.m_XGuid.stGuid2.Data1;
			m_XGuid.stGuid2.Data2 = rhs.m_XGuid.stGuid2.Data2;
			return *this;
		}

		static_assert(sizeof(XGuid::GUID_1) == sizeof(XGuid::GUID_2), "error guid type!");
    };
}