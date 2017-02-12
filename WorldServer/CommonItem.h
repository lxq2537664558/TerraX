#pragma once

#include "CommonItem.h"

namespace TerraX
{
    class CommonOwner;
    class CommonField
    {
    public:
        template <typename T>
        T GetValue()
        {
            if (m_eFieldType == FieldType_t::echar32) {
                return 0;
            }

            return *((T*)pData);
        }
		void Fill(FieldType_t eFiledType, int offset, char* pDataBuffer)
		{
			m_eFieldType = eFiledType;
			pData = pDataBuffer + offset;
		}
    private:
        FieldType_t m_eFieldType;
        char* pData{nullptr};
    };

    template <int BUFFER_SIZE, int FIELD_COUNT>
    class CommonItem
    {
    public:
        CommonItem() 
		{
			for (int i = 0; i < FIELD_COUNT; ++i)
			{
				Fill(i, m_Fields[i]);
			}
		}
        void SetCommonOwner(CommonOwner* pOwner) { m_pCommonOwner = pOwner; }

		virtual void LoadFromDB() = 0;
		virtual void Fill(int nFieldIndex, CommonField& rField) = 0;

        template <typename T>
        T GetValue(int nFieldIndex)
        {
            return m_Fields[nFieldIndex].GetValue<T>();
        }
        template <typename T>
        T SetValue(int nFieldIndex, T&& val)
        {
            auto value = std::move(val);
        }

    protected:
        CommonOwner* m_pCommonOwner{nullptr};
        char m_DataBuffer[BUFFER_SIZE]{0};
        CommonField m_Fields[FIELD_COUNT]{0};
    };

	//make all field as data buffer, use offset to find it 
	//redis: setrange will be useful
	static const int ACCOUNT_ITEM_BUFFER_SIZE = 246;
	class AccountItem : public CommonItem<ACCOUNT_ITEM_BUFFER_SIZE, account_fieldcount>
	{
	public:
		AccountItem() {
		}
		void LoadFromDB() override final {
			//m_pCommonOwner
			//redisReply* reply = (redisReply*)redisCommand(c, "GET Owner:$OwnerID:AccountItem");
			//mempcy(m_DataBuffer, reply->str, size_data);
			//freeReplyObject(reply);
		}
		void ReadFromBuffer() {}
		void Save2DB() {}
		void Sync2Server() {}
		void Sync2Client() {}

		void Fill(int nFieldIndex, CommonField& rField) override final {
			rField.Fill(static_cast<FieldType_t>(stAccountItemProperty[nFieldIndex].nFieldType), stAccountItemProperty[nFieldIndex].nFieldOffset,
				m_DataBuffer);
		}

	private:
	};

}