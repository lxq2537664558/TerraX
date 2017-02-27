#pragma once
#include <array>
#include "Avatar.h"
#include "base_iterator.hpp"

namespace TerraX
{
    class Avatar;
    class MissionItem;

    class MissionDB_Monitor : public ISubject<DataType_t, int, Avatar*>
    {
    public:
        void OnDataChanged_Mission();
        void OnMissionCreated(MissionItem* pMissionItem, Avatar* pTarget);
        void OnMissionRemoved(MissionItem* pMissionItem, Avatar* pTarget);
    };
    class MissionItem
    {
    private:
        TerraXGuid m_Guid;
        int m_nMissionID{0};
        int m_nMissionState{0};
        Avatar* m_pAvatar{nullptr};
        MissionDB_Monitor* m_pMonitor{nullptr};

    public:
        MissionItem(Avatar* pAvatar, MissionDB_Monitor* pMonitor) : m_pAvatar(pAvatar), m_pMonitor(pMonitor)
        {
            m_Guid = TerraXGuid::Generate();
        };
        ~MissionItem() = default;
        void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

        void SetMissionID(int nMissionID) { m_nMissionID = nMissionID; }
        int GetMissionID() const { return m_nMissionID; }

        void SetMissionState(int nMissionState) { m_nMissionState = nMissionState; }
        int GetMissionState() const { return m_nMissionState; }
    };



    const int MAX_MISSION_COUNT = 256;

    class MissionDB
    {
    private:
        Avatar* m_pAvatar{nullptr};
		Container_T<MissionItem, MAX_MISSION_COUNT> m_MissionContainer;
        std::unique_ptr<MissionDB_Monitor> m_pDBMonitor;

    public:
        MissionDB() { m_pDBMonitor.reset(new MissionDB_Monitor()); }
        void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

		MissionItem* CreateMission(int nMissionID);
        void RemoveMission(int nMissionID);

		MissionItem* GetMission(int nMissionID);
		Container_T<MissionItem, MAX_MISSION_COUNT>& GetMissionContainer() { return m_MissionContainer; }
    };

    class MissionComponent : public IObserver<DataType_t, int, Avatar*>
    {
    private:
        Avatar* m_pOwner{nullptr};  // weak_ptr
        std::unique_ptr<MissionDB> m_pMissionDB;

    public:
        MissionComponent(Avatar* pAvatar)
        {
            m_pOwner = pAvatar;

            m_pMissionDB.reset(new MissionDB());
            m_pMissionDB->SetOwner(pAvatar);
        }

        void OnUpdate(DataType_t eDataType, int value, Avatar* pTarget) override;
    };
}