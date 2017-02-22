#include "Avatar.h"
#include "AvatarComponent.h"
#include "MissionComponent.h"
#include "AchievementComponent.h"
#include "CoinComponent.h"
#include "ItemComponent.h"
#include "SkillComponent.h"
#include "DataSyncComponent.h"

using namespace TerraX;

Avatar::Avatar()
{}

Avatar::~Avatar()
{

}

void Avatar::InitComponents()
{
	m_pAvatarComponent.reset(new AvatarComponent(this));
	m_pMissionComponent.reset(new MissionComponent(this));
	m_pAchievementComponent.reset(new AchievementComponent(this));
	m_pCoinComponent.reset(new CoinComponent(this));
	m_pItemComponent.reset(new ItemComponent(this));
	m_pSkillComponent.reset(new SkillComponent(this));
	m_pDataSyncComponent.reset(new DataSyncComponent(this));

	
	m_pAvatarComponent->GetAvatarDB()->AddObserver(m_pMissionComponent.get());
	m_pAvatarComponent->GetAvatarDB()->AddObserver(m_pAchievementComponent.get());
	m_pAvatarComponent->GetAvatarDB()->AddObserver(m_pDataSyncComponent.get());
}