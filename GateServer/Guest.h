#pragma once
#include <memory>
#include <string>
#include <map>
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
	class GateAccount;
	class Guest
	{
	public:
		Guest(int GuestID);
		~Guest() {}

		void InitAccountInfo(GateAccount* pAccount) { m_pCurrentAccount.reset(pAccount); }

		void AttachAvatar(int32_t nAvatarID) { m_nCurrentAvatarID = nAvatarID; }
		int32_t GetAttachedAvatarID() { return m_nCurrentAvatarID; }

		void SetGuestGameState(GuestState_t eGameState) { m_eGameState = eGameState; }
		GuestState_t GetGuestGameState() { return m_eGameState; }

		int32_t GetGuestID() const { return m_nGuestID; }
		int32_t GetDestPeerInfo(PeerType_t ePeerType);
	private:
		void InitCenterConnection();

	private:
		int32_t m_nGuestID{ 0 };
		int32_t m_nCurrentAvatarID{ 0 };
		GuestState_t m_eGameState{ GuestState_t::eGameState_NULL };
		std::map<PeerType_t, int> m_mapPeerInfos;
		std::unique_ptr<GateAccount> m_pCurrentAccount;
	};


}