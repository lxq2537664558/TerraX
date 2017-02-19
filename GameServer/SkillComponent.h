#pragma once
#include "Avatar.h"

namespace TerraX
{
	class Avatar;


	class SkillComponent
	{
	public:
		SkillComponent(Avatar* pAvatar) {
			m_pOwner = pAvatar;
		}
	private:
		Avatar* m_pOwner{ nullptr }; //weak_ptr
	};

}