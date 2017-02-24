#include "AvatarComponent.h"

using namespace TerraX;

void AvatarDB_Monitor::OnDataChanged_Exp(int nExp, Avatar* pTarget)
{
	Notify(DataType_t::eAvatar_Exp, nExp, pTarget);
}

void AvatarDB_Monitor::OnDataChanged_Level(int nLevel, Avatar* pTarget)
{
	Notify(DataType_t::eAvatar_Level, nLevel, pTarget);
}


//////////////////////////////////////////////////////////////////////////

void AvatarComponent::SetExp(int nExp)
{
	if (nExp >= LEVEL_UP_EXP) {
		int nLevel = m_pAvatarDB->GetAvatarItem()->GetLevel();
		if (nLevel + 1 >= LEVEL_MAX) {
			nExp = LEVEL_UP_EXP;
		}
		else {
			nExp = nExp - LEVEL_UP_EXP;
			nLevel += 1;
			m_pAvatarDB->GetAvatarItem()->SetLevel(nLevel);
		}
		m_pAvatarDB->GetAvatarItem()->SetExp(nExp);
	}
	else
	{
		m_pAvatarDB->GetAvatarItem()->SetExp(nExp);
	}
}

void AvatarComponent::SetLevel(int nLevel)
{
	if (nLevel <= 0 || nLevel >= LEVEL_MAX)
	{
		return;
	}
	m_pAvatarDB->GetAvatarItem()->SetLevel(nLevel);
}