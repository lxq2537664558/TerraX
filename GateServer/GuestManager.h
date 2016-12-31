#pragma once
#include <cassert>
#include <map>
#include <memory>
#include <string>
#include <unordered_map>
#include "ComDef.h"
#include "GateAccount.h"
#include "Guest.h"
namespace TerraX
{
	class GuestManager
	{
		NOCOPY(GuestManager);
		MAKEINSTANCE(GuestManager);

	public:
		GuestManager() = default;
		~GuestManager() = default;

		Guest* GetGuest(int32_t nGuestID)
		{
			auto it = m_mapGuests.find(nGuestID);
			return it == m_mapGuests.end() ? nullptr : it->second.get();
		}
		void CreateGuest(int32_t nGuestID)
		{
			m_mapGuests[nGuestID] = std::move(std::unique_ptr<Guest>(new Guest(nGuestID)));
		}

	private:
		std::unordered_map<int32_t, std::unique_ptr<Guest> > m_mapGuests;
	};
}