#pragma once

#include <cstdint>
namespace TerraX
{
	
	union XGuid
	{
		struct GUID_1 {
			unsigned long  Data1;
			unsigned short Data2;
			unsigned short Data3;
			unsigned char  Data4[8];
		} stGuid;
		struct GUID_2 {
			uint64_t Data1;
			uint64_t Data2;
		};
		char szGuid[sizeof GUID_1];
	};

	static_assert(sizeof(XGuid::GUID_1) == sizeof(XGuid::GUID_2), "error guid type!");
}