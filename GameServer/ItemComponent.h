#pragma once
#include "Avatar.h"

namespace TerraX
{
	class Avatar;

	class ItemComponent
	{
	public:
		ItemComponent(Avatar* pAvatar) {
			m_pOwner = pAvatar;
		}
	private:
		Avatar* m_pOwner{ nullptr }; //weak_ptr
	};
}