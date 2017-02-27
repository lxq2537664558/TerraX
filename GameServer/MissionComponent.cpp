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
MissionItem* MissionDB::CreateMission(int nMissionID) {
	if (m_MissionContainer.Size() >= MAX_MISSION_COUNT) {
		return nullptr;
	}
	auto pItem = new MissionItem(m_pAvatar, m_pDBMonitor.get());
	pItem->SetMissionID(nMissionID);
	m_MissionContainer.Push(pItem);
	//m_pDBMonitor->OnMissionCreated(pItem, m_pAvatar);
	return pItem;
}

void MissionDB::RemoveMission(int nMissionID)
{
	auto iter = m_MissionContainer.Iterator();
	int nIndex = 0;
	while (!iter.IsEnd())
	{
		auto pItem = iter.Next();
		if (pItem->GetMissionID() == nMissionID)
		{
			m_MissionContainer.RemoveAt(nIndex);
			//m_pDBMonitor->OnMissionRemoved(m_pMissionItems[i].get(), m_pAvatar);
			return;
		}
		++nIndex;
	}
}

MissionItem* MissionDB::GetMission(int nMissionID)
{
	auto iter = m_MissionContainer.Iterator();
	int nIndex = 0;
	while (!iter.IsEnd())
	{
		auto pItem = iter.Next();
		if (pItem->GetMissionID() == nMissionID)
		{
			return pItem;
		}
		++nIndex;
	}
	return nullptr;
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