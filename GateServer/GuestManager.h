#pragma once
#include <string>
#include <map>
#include <memory>
#include "ComDef.h"
#include <cassert>


enum class Guest_GameState_t
{
	eGameState_NULL,
	eCheckingPermission,
	eChosingAvatar,
	eEnteringScene,
	eGaming,
};

class Guest
{
public:
	Guest(const char* szAccountName) : m_strAccountName(szAccountName),
	m_eGameState(Guest_GameState_t::eCheckingPermission){}
	~Guest(){}

	void PossessedAvatar(int32_t nAvatarID) { m_nPossessedAvatarID = nAvatarID; }
	void SetGuestGameState(Guest_GameState_t eGameState) { m_eGameState = eGameState; }
private:
	std::string m_strAccountName;
	int32_t m_nPossessedAvatarID{ 0 };
	Guest_GameState_t m_eGameState{ Guest_GameState_t::eGameState_NULL };
};


class GuestManager
{
	NOCOPY(GuestManager);
	MAKEINSTANCE(GuestManager);
public:
	GuestManager() = default;
	~GuestManager() = default;

	void AddGuest(int32_t nGuestID, const char* szAccountName) {
		m_mapGuests[nGuestID] = std::unique_ptr<Guest>(new Guest(szAccountName));
	}
	void SetGuestAvatarID(int32_t nGuestID, int32_t nAvatarID) {
		auto it = m_mapGuests.find(nGuestID);
		assert(it != m_mapGuests.end());
		it->second->PossessedAvatar(nAvatarID);
	}
	void SetGuestGameState(int32_t nGuestID, Guest_GameState_t eGameState) {
		auto it = m_mapGuests.find(nGuestID);
		assert(it != m_mapGuests.end());
		it->second->SetGuestGameState(eGameState);
	}
private:
	std::map<int32_t, std::unique_ptr<Guest> > m_mapGuests;
};