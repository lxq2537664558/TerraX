#include "AvatarComponent.h"

using namespace TerraX;

void AvatarDB_Monitor::OnDataChanged_Exp(int nExp, Avatar* pTarget)
{
	if (nExp >= LEVEL_UP_EXP) {
		int nLevel = pTarget->GetAvatarComponent()->GetAvatarDB()->GetAvatarItem()->GetLevel();
		if (nLevel + 1 >= LEVEL_MAX) {
			nExp = LEVEL_UP_EXP;
		}
		else {
			nExp = nExp - LEVEL_UP_EXP;
			nLevel += 1;
			pTarget->GetAvatarComponent()->GetAvatarDB()->GetAvatarItem()->SetLevel(nLevel);
		}
		pTarget->GetAvatarComponent()->GetAvatarDB()->GetAvatarItem()->SetExp(nExp);
	}
	else {
		Notify(DataType_t::eAvatar_Exp, nExp, pTarget);
	}
}

void AvatarDB_Monitor::OnDataChanged_Level(int nLevel, Avatar* pTarget)
{
	if (nLevel <= LEVEL_MAX) {
		Notify(DataType_t::eAvatar_Level, nLevel, pTarget);
	}
}