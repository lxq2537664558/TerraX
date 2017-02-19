#pragma once
#include "Avatar.h"
#include <array>
namespace TerraX
{
    class Avatar;

	class MissionItem
	{
	private:
		XGuid m_Guid{ 0 };
		int m_nMissionID{ 0 };
		int m_nMissionState{ 0 };
		Avatar* m_pAvatar{ nullptr };
		MissionDB_Monitor* m_pMonitor{ nullptr };
	public:
		MissionItem(Avatar* pAvatar, MissionDB_Monitor* pMonitor) :
			m_pAvatar(pAvatar),
			m_pMonitor(pMonitor) {
			/*m_Guid = GUID;*/
		};
		~MissionItem() = default;
		void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

		void SetMissionID(int nMissionID) { m_nMissionID = nMissionID; }
		int GetMissionID() const { return m_nMissionID; }

		void SetMissionState(int nMissionState) { m_nMissionState = nMissionState; }
		int GetMissionState() const { return m_nMissionState; }
	};

    class MissionDB_Monitor : public ISubject<DataType_t, int, Avatar*>
    {
	public:
		void OnDataChanged_Mission()
		{
			//Notify(DataType_t::eAvatar_Exp, 0, nullptr);
		}
		void OnMissionCreated(MissionItem* pMissionItem, Avatar* pTarget)
		{

		}
		void OnMissionRemoved(MissionItem* pMissionItem, Avatar* pTarget)
		{

		}
    };

    class MissionDB
    {
    private:
		static const int MAX_ITEM_COUNT = 256;

		Avatar* m_pAvatar{ nullptr };
		std::array<std::unique_ptr<MissionItem>, MAX_ITEM_COUNT> m_pMissionItems;
        std::unique_ptr<MissionDB_Monitor> m_pDBMonitor;

    public:
        MissionDB() { m_pDBMonitor.reset(new MissionDB_Monitor()); }
		void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

        MissionItem* CreateMission(int nMissionID)
        {
            for (int i = 0; i < MAX_ITEM_COUNT; i++) {
                if (m_pMissionItems[i] == nullptr) {
					m_pMissionItems[i].reset(new MissionItem(m_pAvatar, m_pDBMonitor.get()));
                    m_pMissionItems[i]->SetMissionID(nMissionID);

					m_pDBMonitor->OnMissionCreated(m_pMissionItems[i].get(), m_pAvatar);
                    return m_pMissionItems[i].get();
                }
            }
            return nullptr;
        }
        void RemoveMission(int nMissionID) {
			for (int i = 0; i < MAX_ITEM_COUNT; i++) {
				if (m_pMissionItems[i] != nullptr && m_pMissionItems[i]->GetMissionID() == nMissionID) {
					m_pMissionItems[i].reset();
					m_pDBMonitor->OnMissionRemoved(m_pMissionItems[i].get(), m_pAvatar);
					return;
				}
			}
		}

        //MissionItem* GetMission() { return nullptr; }
    };

    class MissionComponent : public IObserver<DataType_t, int, Avatar*>
    {
    public:
        MissionComponent(Avatar* pAvatar) { 
			m_pOwner = pAvatar; 

			m_pMissionDB.reset(new MissionDB());
			m_pMissionDB->SetOwner(pAvatar);
		}

        void OnUpdate(DataType_t eDataType, int value, Avatar* pTarget) override
        {
            if (eDataType == DataType_t::eAvatar_Level) {
                if (value > 0) {
                    std::cout << "Level Up, Mission Complete!" << std::endl;
                }
            }
        }

    private:
        Avatar* m_pOwner{nullptr};  // weak_ptr
		std::unique_ptr<MissionDB> m_pMissionDB;
    };
}