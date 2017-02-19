#pragma once
#include "Avatar.h"

namespace TerraX
{
    class AvatarItem
    {
    private:
        int m_nExp{0};
        int m_nLevel{0};
        Avatar* m_pAvatar{nullptr};
        AvatarDB_Monitor* m_pMonitor{nullptr};

    public:
        AvatarItem(Avatar* pAvatar, AvatarDB_Monitor* pMonitor) : m_pAvatar(pAvatar), m_pMonitor(pMonitor) {}
        ~AvatarItem() = default;

        void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

        int GetExp() const { return m_nExp; }
        void SetExp(int nExp)
        {
            m_nExp = nExp;
            m_pMonitor->OnDataChanged_Exp(nExp, m_pAvatar);
        }

        int GetLevel() const { return m_nLevel; }
        void SetLevel(int nLevel)
        {
            m_nLevel = nLevel;
            m_pMonitor->OnDataChanged_Level(m_nLevel, m_pAvatar);
        }
    };

    class AvatarDB_Monitor : public ISubject<DataType_t, int, Avatar*>
    {
    public:
        void OnDataChanged_Exp(int nExp, Avatar* pTarget);
        void OnDataChanged_Level(int nLevel, Avatar* pTarget);

    private:
        static const int LEVEL_UP_EXP = 1000;
        static const int LEVEL_MAX = 100;
    };

    class AvatarDB
    {
    private:
        Avatar* m_pAvatar{nullptr};
        std::unique_ptr<AvatarDB_Monitor> m_pMonitor;
        std::unique_ptr<AvatarItem> m_pAvatarItem;

    public:
        AvatarDB()
        {
            m_pMonitor.reset(new AvatarDB_Monitor());
            m_pAvatarItem.reset(new AvatarItem(m_pAvatar, m_pMonitor.get()));
        }
        void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

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

            m_pAvatarDB.reset(new AvatarDB());
            m_pAvatarDB->SetOwner(m_pOwner);
        }

        AvatarDB* GetAvatarDB() { return m_pAvatarDB.get(); }

    private:
        Avatar* m_pOwner{nullptr};  // weak_ptr
        std::unique_ptr<AvatarDB> m_pAvatarDB;
    };
}