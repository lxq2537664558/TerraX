#pragma once

#include "Avatar.h"

namespace TerraX
{
	class Avatar;
	
	//datacomponet--item--field:value--updatetype--srcobj--tarobj
	//sync_flag: client, aoi, gate, game, world, db, roleattr, rolelist
	//opt_flag: cover, update,
	//dirty_flag: clear_dirty
	class AchievementDataSync {};
	class AvatarDataSync{};
	class CoinDataSync{};
	class ItemDataSync{};
	class MissionDataSync{};
	class SkillDataSync{};

	class IDataSync
	{
		void Sync2Aoi(bool bIncludeMe) {}
		void Sync2Client() {}
		void Sync2Gate() {}
		void Sync2Game() {}
		void Sync2DB() {}
		void SyncWorld() {}
	};
	class DataSyncComponent : public IObserver<DataType_t, int, Avatar*>
	{
	public:
		DataSyncComponent(Avatar* pAvatar) {
			m_pOwner = pAvatar;
		}

		void SyncAllData() {}
		void OnUpdate(DataType_t eDataType, int value, Avatar* pTarget) override
		{
		}

	private:
		Avatar* m_pOwner{ nullptr }; //weak_ptr
	};
}