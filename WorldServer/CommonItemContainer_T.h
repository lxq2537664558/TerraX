#pragma once

namespace TerraX
{
	class CommonOwner;

	template<class TItem, int MAX_COUNT>
	class CommonItemContainer_T
	{
	public:
		CommonItemContainer_T() {

		}
		void SetCommonOwner(CommonOwner* pOwner) { m_pCommonOwner = pOwner; }
		void AddItem(CommonItem* pItem, int index = -1) {}
	private:
		TItem m_Items[MAX_COUNT]{ 0 };
		CommonOwner* m_pCommonOwner{ nullptr };
	};
}