#pragma once

#include "CommonItemDefine.h"
#include "CommonItem.h"
#include "CommonItemContainer_T.h"

namespace TerraX
{
	class ICommonDataComponent
	{

	};

	class Avatar
	{

	private:
		ICommonDataComponent* m_pCommonDataComponent; //
	};
	/*
	static const int AVATAR_ITEM_BUFFER_SIZE = 2048;
	class AvatarItem : public CommonItem<AVATAR_ITEM_BUFFER_SIZE, 1>
	{

	};

	static const int ITEM_BUFFER_SIZE = 1234;
	class Item : public CommonItem<ITEM_BUFFER_SIZE, 1>
	{

	};
	*/
#define GET_FIELD_VALUE(common_item, field_type, field_name, def_val) \
	common_item->GetValue<decltype(def_val)>(static_cast<int>(field_type::field_name), def_val);

#define SET_FIELD_VALUE(common_item, field_type, field_name, val) \
	common_item->SetValue<decltype(val)>(static_cast<int>(field_type::field_name), def_val);

	
	
	class AvatarDataComponent : public ICommonDataComponent
	{
	public:

	private:
		//AvatarItem m_AvatarItem;
		//CommonItemContainer_T<Item, 1000> m_ItemDatas;
	};

	class AccountDataComponent : public ICommonDataComponent
	{
	public:
		AccountDataComponent() /*: m_AccountItem, m_AvatarDatas("AvatarDatas")*/ {
			// owner:account_id:
		}
		//void SetCommonOwner(){}
		void Init() {
		}

	private:
		AccountItem m_AccountItem;
		//CommonItemSet_T<AvatarItem, 6> m_AvatarDatas;
	};

	class Subject{};
	class Observer{};
	class CommonItemManager : public Subject
	{
		//instance
	public:
		void OnItemCreate(class CommonItem* pCommonItem, CommonOwner* pObj ) {
		}
	};
}