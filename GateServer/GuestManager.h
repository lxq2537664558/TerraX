#pragma once
#include <cassert>
#include <memory>
#include <string>
#include <unordered_map>
#include "base_macro.h"
#include "GateAccount.h"
#include "Guest.h"
namespace TerraX
{
	class GuestManager
	{
		DISABLE_COPY(GuestManager);
		MAKE_INSTANCE(GuestManager);

	public:
		GuestManager() = default;
		~GuestManager() = default;

		Guest* GetGuest(int32_t nGuestID)
		{
			auto it = m_mapGuests.find(nGuestID);
			return it == m_mapGuests.end() ? nullptr : it->second.get();
		}
		Guest* CreateGuest(int32_t nGuestID)
		{
			m_mapGuests[nGuestID].reset(new Guest(nGuestID));
			return m_mapGuests[nGuestID].get();
		}

	private:
		std::unordered_map<int32_t, std::unique_ptr<Guest> > m_mapGuests;
	};
}