#pragma once
#include "Avatar.h"
#include <array>
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


	class MissionIterator;
	template<int CONTAINER_SIZE = 1>
	class MissionContainer : public BaseAggregate<MissionItem>
	{
	private:
		std::array<std::unique_ptr<MissionItem>, CONTAINER_SIZE> m_pMissionItems;
		std::unique_ptr<BaseIterator<MissionItem>> m_pIterator{ nullptr };

	public:
		MissionContainer() {
			
		}
		int Count() override {
			return m_pMissionItems.size();
		}
		void Push(MissionItem&& T) override {

		}
		MissionItem* Pop(const int nIndex) override {
			return nullptr;
		}
		BaseIterator<MissionItem>* Iterator() {
			m_pIterator.reset(new MissionIterator(this));
			return m_pIterator.get();
		}
	};


	const int MAX_MISSION_COUNT = 256;
	class MissionIterator : public BaseIterator<MissionItem>
	{
	private:
		MissionContainer<MAX_MISSION_COUNT>* m_pAggregate{ nullptr };
		int m_nCurIndex{ 0 };
	public:
		MissionIterator(MissionContainer<MAX_MISSION_COUNT>* pAggregate) : m_pAggregate(pAggregate){}

		MissionItem* Begin() override {
			return m_pAggregate->Pop(0);
		}

		MissionItem* Next() override {
			++m_nCurIndex;
			if (m_nCurIndex < m_pAggregate->Count())
			{
				return m_pAggregate->Pop(m_nCurIndex);
			}
			return nullptr;
		}

		MissionItem* Current() override {
			return m_pAggregate->Pop(m_nCurIndex);
		}

		bool IsEnd() override {
			return m_nCurIndex >= m_pAggregate->Count();
		}
	};


    class MissionDB
    {
    private:

		Avatar* m_pAvatar{ nullptr };
		std::array<std::unique_ptr<MissionItem>, MAX_MISSION_COUNT> m_pMissionItems;
        std::unique_ptr<MissionDB_Monitor> m_pDBMonitor;

    public:
        MissionDB() { m_pDBMonitor.reset(new MissionDB_Monitor()); }
		void SetOwner(Avatar* pAvatar) { m_pAvatar = pAvatar; }

		MissionItem* CreateMission(int nMissionID);
		void RemoveMission(int nMissionID);

        //MissionItem* GetMission() { return nullptr; }
    };

    class MissionComponent : public IObserver<DataType_t, int, Avatar*>
	{
	private:
		Avatar* m_pOwner{ nullptr };  // weak_ptr
		std::unique_ptr<MissionDB> m_pMissionDB;

    public:
        MissionComponent(Avatar* pAvatar) { 
			m_pOwner = pAvatar; 

			m_pMissionDB.reset(new MissionDB());
			m_pMissionDB->SetOwner(pAvatar);
		}

		void OnUpdate(DataType_t eDataType, int value, Avatar* pTarget) override;
    };
}