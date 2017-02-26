#include "MissionComponent.h"

using namespace TerraX;

void MissionDB_Monitor::OnDataChanged_Mission()
{
    // Notify(DataType_t::eAvatar_Exp, 0, nullptr);
}

void MissionDB_Monitor::OnMissionCreated(MissionItem* pMissionItem, Avatar* pTarget) {}

void MissionDB_Monitor::OnMissionRemoved(MissionItem* pMissionItem, Avatar* pTarget) {}

//////////////////////////////////////////////////////////////////////////



//////////////////////////////////////////////////////////////////////////
MissionItem* MissionDB::CreateMission(int nMissionID)
{
    for (int i = 0; i < MAX_MISSION_COUNT; i++) {
        if (m_pMissionItems[i] == nullptr) {
            m_pMissionItems[i].reset(new MissionItem(m_pAvatar, m_pDBMonitor.get()));
            m_pMissionItems[i]->SetMissionID(nMissionID);

            // m_pDBMonitor->OnMissionCreated(m_pMissionItems[i].get(), m_pAvatar);
            return m_pMissionItems[i].get();
        }
    }
    return nullptr;
}

void MissionDB::RemoveMission(int nMissionID)
{
    for (int i = 0; i < MAX_MISSION_COUNT; i++) {
        if (m_pMissionItems[i] != nullptr && m_pMissionItems[i]->GetMissionID() == nMissionID) {
            m_pMissionItems[i].reset();
            // m_pDBMonitor->OnMissionRemoved(m_pMissionItems[i].get(), m_pAvatar);
            return;
        }
    }
}

//////////////////////////////////////////////////////////////////////////

void MissionComponent::OnUpdate(DataType_t eDataType, int value, Avatar* pTarget)
{
	if (eDataType == DataType_t::eAvatar_Level) {
		if (value > 0) {
			//m_pMissionDB->
			std::cout << "Level Up, Mission Complete!" << std::endl;
		}
	}
}