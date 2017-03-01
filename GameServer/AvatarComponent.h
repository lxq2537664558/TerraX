#pragma once
#include "Avatar.h"

namespace TerraX
{

	const int LEVEL_UP_EXP = 1000;
	const int LEVEL_MAX = 100;

	class AvatarDB_Monitor : public ISubject<DataType_t, int, Avatar*>
	{
	public:
		void OnDataChanged_Exp(int nExp, Avatar* pTarget);
		void OnDataChanged_Level(int nLevel, Avatar* pTarget);

	};

	enum FieldSyncFlag_t
	{
		eDirty_Client,
		eDirty_ClientAOI,
		eDirty_World,
		eDirty_DB,

		eClient = 16,
		eClientAOI,
		eWorld,
		eDB,

		eFieldSyncFlag_Size = 32,
	};
	struct fieldproperty
	{
		int offset;/
		int syncflag;
		char fieldtype[32];
	};
	fieldproperty stfp[10] = { {0,0,"int32",},{ 1,1,"char64"} };
    class AvatarItem
    {
    private:
        int m_nExp{0};
		int m_nExpFlag{ 0 };
        int m_nLevel{0};
        Avatar* m_pAvatar{nullptr};
        AvatarDB_Monitor* m_pMonitor{nullptr};

    public:
        AvatarItem(Avatar* pAvatar, AvatarDB_Monitor* pMonitor) : m_pAvatar(pAvatar), m_pMonitor(pMonitor) {}
        ~AvatarItem() = default;

        void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

		void ExtractData(FieldSyncFlag_t eFlag){}

		static const int kField_AvatarItem_Exp = 1;
        int GetExp() const { return m_nExp; }
        void SetExp(int nExp)
        {
            m_nExp = nExp;
			m_nExpFlag = m_nExpFlag | 0x0000FFFF;
            m_pMonitor->OnDataChanged_Exp(nExp, m_pAvatar);
        }

        int GetLevel() const { return m_nLevel; }
        void SetLevel(int nLevel)
        {
            m_nLevel = nLevel;
            m_pMonitor->OnDataChanged_Level(m_nLevel, m_pAvatar);
        }
    };

    class AvatarDB
    {
    private:
        Avatar* m_pAvatar{nullptr};
        std::unique_ptr<AvatarDB_Monitor> m_pMonitor;
        std::unique_ptr<AvatarItem> m_pAvatarItem;

    public:
        AvatarDB(Avatar* pAvatar) : m_pAvatar(pAvatar)
        {
            m_pMonitor.reset(new AvatarDB_Monitor());
            m_pAvatarItem.reset(new AvatarItem(m_pAvatar, m_pMonitor.get()));
        }

        void AddObserver(IObserver<DataType_t, int, Avatar*>* pObserver)
        {
            m_pMonitor->AddObserver(pObserver);
        }

        AvatarItem* GetAvatarItem() { return m_pAvatarItem.get(); }
    };

    class AvatarComponent
    {
    public:
        AvatarComponent(Avatar* pAvatar)
        {
            m_pOwner = pAvatar;

            m_pAvatarDB.reset(new AvatarDB(pAvatar));
        }

        AvatarDB* GetAvatarDB() { return m_pAvatarDB.get(); }


		int GetExp() const { return m_pAvatarDB->GetAvatarItem()->GetExp(); }
		void SetExp(int nExp);

		int GetLevel() const { return m_pAvatarDB->GetAvatarItem()->GetLevel(); }
		void SetLevel(int nLevel);
    private:
        Avatar* m_pOwner{nullptr};  // weak_ptr
        std::unique_ptr<AvatarDB> m_pAvatarDB;
    };
}