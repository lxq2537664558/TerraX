#pragma once

#include "CommonItem.h"

namespace TerraX
{
	class CommonOwner;
	enum class FieldType_t
	{
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
	class CommonField
	{
	public:
		template<typename T>
		T GetValue() {
			if (m_eFieldType == FieldType_t::echar32)
			{
				return 0;
			}

			return *((T*)*pData);
		}
	private:
		FieldType_t m_eFieldType;
		char* pData{ nullptr };
	};

	template<int BUFFER_SIZE, int FIELD_COUNT>
	class CommonItem
	{
	public:
		void SetCommonOwner(CommonOwner* pOwner) { m_pCommonOwner = pOwner; }
		
		template<typename T>
		T GetValue(int nFieldIndex) { return m_Fields[nFieldIndex].GetValue<T>(); }
		template<typename T>
		T SetValue(int nFieldIndex, T&& val) { auto value = std::move(val); }
	
	protected:
		CommonOwner* m_pCommonOwner{ nullptr };
		char m_DataBuffer[BUFFER_SIZE]{ 0 };
		CommonField m_Fields[FIELD_COUNT]{ 0 };
	};
}