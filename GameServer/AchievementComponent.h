#pragma once
#include "Avatar.h"

namespace TerraX
{
	class Avatar;
	class AchievementComponent : public IObserver<DataType_t, int, Avatar*>
	{
	public:
		AchievementComponent(Avatar* pAvatar) {
			m_pOwner = pAvatar;
		}

		void OnUpdate(DataType_t eDataType, int value, Avatar* pTarget) override
		{
		}
	private:
		Avatar* m_pOwner{ nullptr }; //weak_ptr
	};
}