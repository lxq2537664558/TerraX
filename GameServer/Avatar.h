#pragma once

#include <memory>
#include "observer.hpp"
#include "base_type.h"

namespace TerraX
{

	enum class DataType_t
	{
		eAvatar_Exp,
		eAvatar_Level,
	};

	class AvatarComponent;
	class MissionComponent;
	class AchievementComponent;
	class CoinComponent;
	class ItemComponent;
	class SkillComponent;
	class DataSyncComponent;

	class Avatar
	{
	public:
		Avatar();
		~Avatar();

		void InitComponents();
		AvatarComponent* GetAvatarComponent() { return m_pAvatarComponent.get(); }
		MissionComponent* GetMissionComponent() { return m_pMissionComponent.get(); }
		AchievementComponent* GetAchievementComponent() { return m_pAchievementComponent.get(); }
		CoinComponent* GetCoinComponent() { return m_pCoinComponent.get(); }
		ItemComponent* GetItemComponent() { return m_pItemComponent.get(); }
		SkillComponent* GetSkillComponent() { return m_pSkillComponent.get(); }
		DataSyncComponent* GetDataSyncComponent() { return m_pDataSyncComponent.get(); }
	private:
		std::unique_ptr<AvatarComponent> m_pAvatarComponent;
		std::unique_ptr<MissionComponent> m_pMissionComponent;
		std::unique_ptr<AchievementComponent> m_pAchievementComponent;
		std::unique_ptr<CoinComponent> m_pCoinComponent;
		std::unique_ptr<ItemComponent> m_pItemComponent;
		std::unique_ptr<SkillComponent> m_pSkillComponent;
		std::unique_ptr<DataSyncComponent> m_pDataSyncComponent;
	};
}