#pragma once
#include <string>
#include <memory>
#include "ComDef.h"
#include <cassert>
#include <map>
#include <unordered_map>
#include "NetDefine.h"
#include "GateAccount.h"

namespace TerraX
{
	enum class GuestState_t
	{
		eGameState_NULL,
		eWaitingAccountInfo,
		eCheckingPermission,
		eChosingAvatar,
		eEnteringScene,
		eGaming,
	};

	class Guest
	{
	public:
		Guest(int GuestID) : m_nGuestID(GuestID), m_eGameState(GuestState_t::eWaitingAccountInfo){}
		~Guest() {}

		void SetAccountInfo(std::unique_ptr<GateAccount>& pAccountInfo) { m_pAccountInfo = std::move(pAccountInfo); }
		void AttachAvatar(int32_t nAvatarID) { m_nAttachAvatarID = nAvatarID; }
		int32_t GetAttachAvatarID() { return m_nAttachAvatarID; }
		void SetGuestGameState(GuestState_t eGameState) { m_eGameState = eGameState; }
		GuestState_t GetGuestGameState() { return m_eGameState; }
		int32_t GetGuestID() const { return m_nGuestID; }
		int32_t GetDestPeerInfo(PeerType_t ePeerType) const {
			return 0;
		}
	private:
		std::string m_strAccountName;
		int32_t m_nGuestID{ 0 };
		int32_t m_nAttachAvatarID{ 0 };
		GuestState_t m_eGameState{ GuestState_t::eGameState_NULL };
		std::map<PeerType_t, int> m_mapPeerInfos;
		std::unique_ptr<GateAccount> m_pAccountInfo;
	};


	class GuestManager
	{
		NOCOPY(GuestManager);
		MAKEINSTANCE(GuestManager);
	public:
		GuestManager() = default;
		~GuestManager() = default;

		Guest* GetGuest(int32_t nGuestID) {
			auto it = m_mapGuests.find(nGuestID);
			return it == m_mapGuests.end() ? nullptr : it->second.get();
		}
		void CreateGuest(int32_t nGuestID) {
			m_mapGuests[nGuestID] = std::move(std::unique_ptr<Guest>(new Guest(nGuestID)));
		}
	private:
		std::unordered_map<int32_t, std::unique_ptr<Guest> > m_mapGuests;
	};
}