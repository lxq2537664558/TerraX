#pragma once

namespace TerraX
{
	enum class CommonItemType_t {
		eCommonItem_Account,
		eCommonItem_Avatar,
		eCommonItem_Item,
	};

	enum AccountItemField_t {
		account_guid,
		accout_name,
		account_createtime,

		account_fieldcount,
	};

	enum FieldType_t {
		ebool,
		echar,
		eint32,
		efloat,
		eint64,
		edouble,
		echar16,
		echar32,
		echar64,
	};
	struct FieldProperty {
		int nFieldType;
		int nFieldOffset;
	};

	const FieldProperty stAccountItemProperty[account_fieldcount] = {
		{echar16, 0}, {echar32, 16}, {eint64, 8} };
}