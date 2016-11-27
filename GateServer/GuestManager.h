#pragma once
#include <string>
#include <map>
#include <memory>
#include "ComDef.h"

class Guest
{
public:
	std::string szAccountName;
	std::int32_t nPossessedPuppetID{ 0 };
};


class GuestManager
{
	NOCOPY(GuestManager);
	MAKEINSTANCE(GuestManager);
public:
	GuestManager() = default;
	~GuestManager() = default;

	void AddGuest(int32_t nGuestID, Guest& guest) {
		m_mapGuests[nGuestID]
	}

private:
	std::map<int32_t, std::unique_ptr<Guest> > m_mapGuests;
};