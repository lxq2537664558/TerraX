#pragma once

#include "Avatar.h"
namespace TerraX
{
	class Avatar;

	class CoinDB
	{
	public:
		int GetMoney() { return m_nMoney; }
		void SetMoney(int nMoney) { m_nMoney = nMoney; }

	private:
		int m_nMoney;
	};


	class CoinComponent
	{
	public:
		CoinComponent(Avatar* pAvatar) {
			m_pOwner = pAvatar;
		}
	private:
		Avatar* m_pOwner{ nullptr }; //weak_ptr
	};
}