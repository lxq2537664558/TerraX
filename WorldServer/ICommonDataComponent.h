#pragma once

#include "CommonItemDefine.h"
#include "CommonItem.h"
#include "CommonItemSet_T.h"

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
	static const int AVATAR_ITEM_BUFFER_SIZE = 2048;
	class AvatarItem : public CommonItem<AVATAR_ITEM_BUFFER_SIZE, 1>
	{

	};

	static const int ITEM_BUFFER_SIZE = 1234;
	class Item : public CommonItem<ITEM_BUFFER_SIZE, 1>
	{

	};

#define GET_FIELD_VALUE(common_item, field_type, field_name, def_val) \
	common_item->GetValue<decltype(def_val)>(static_cast<int>(field_type::field_name), def_val);

#define SET_FIELD_VALUE(common_item, field_type, field_name, val) \
	common_item->SetValue<decltype(val)>(static_cast<int>(field_type::field_name), def_val);

	enum AccountItemField_t
	{
		accout_name,
		account_createtime,

		account_fieldcount,
	};
	//make all field as data buffer, use offset to find it 
	//redis: setrange will be useful
	static const int ACCOUNT_ITEM_BUFFER_SIZE = 246;
	class AccountItem : public CommonItem<ACCOUNT_ITEM_BUFFER_SIZE, 1>
	{
	public:
		void LoadFromDB(){}
		void ReadFromBuffer(){}
		void Save2DB(){}
		void Sync2Server(){}
		void Sync2Client(){}

		
	private:
	};

	class AvatarDataComponent : public ICommonDataComponent
	{
	public:

	private:
		AvatarItem m_AvatarItem;
		CommonItemSet_T<Item, 1000> m_ItemDatas;
	};

	class AccountDataComponent : public ICommonDataComponent
	{
	public:

	private:
		AccountItem m_AccountItem;
		CommonItemSet_T<AvatarItem, 6> m_AvatarDatas;
	};
}