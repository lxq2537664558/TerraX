#pragma once
#include <string>
#include <memory>
#include "ComDef.h"
#include <cassert>
#include <unordered_map>
#include "NetDefine.h"

namespace TerraX
{
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
			m_eGameState(Guest_GameState_t::eCheckingPermission) {}
		~Guest() {}

		void PossessedAvatar(int32_t nAvatarID) { m_nPossessedAvatarID = nAvatarID; }
		void SetGuestGameState(Guest_GameState_t eGameState) { m_eGameState = eGameState; }
		int32_t GetGuestID() const { return m_nGuestID; }
		int32_t GetDestPeerInfo(PeerType_t ePeerType) const {
			if (ePeerType == PeerType_t::worldserver)
			{
				return m_nLinkWorldPeerInfo;
			}
			if (ePeerType == PeerType_t::gameserver)
			{
				return m_nLinkGamePeerInfo;
			}
			return 0;
		}
	private:
		std::string m_strAccountName;
		int32_t m_nGuestID;
		int32_t m_nPossessedAvatarID{ 0 };
		Guest_GameState_t m_eGameState{ Guest_GameState_t::eGameState_NULL };

		int32_t m_nLinkWorldPeerInfo{ 0 };
		int32_t m_nLinkGamePeerInfo{ 0 };
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

		Guest* FindGuest(int32_t nGuestID) {
			auto it = m_mapGuests.find(nGuestID);
			return it == m_mapGuests.end() ? nullptr : it->second.get();
		}
	private:
		std::unordered_map<int32_t, std::unique_ptr<Guest> > m_mapGuests;
	};
}