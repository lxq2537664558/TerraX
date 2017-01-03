#pragma once
#include <functional>
#include <memory>
#include <queue>
#include "ComDef.h"
namespace TerraX
{
    class TimerTask
    {
        NOCOPY(TimerTask);

    public:
        explicit TimerTask(int nTimePoint) : m_nActiveTimePoint(nTimePoint) {}
		~TimerTask() {}
        int GetActiveTimePoint() const { return m_nActiveTimePoint; }

    private:
        int m_nActiveTimePoint{0};
    };

    class TaskManager
    {
        NOCOPY(TaskManager);
        MAKEINSTANCE(TaskManager);
        using TaskPtr = std::unique_ptr<TimerTask>;

    public:
        TaskManager()
            : m_taskqueue([](TaskPtr& t1, TaskPtr& t2) {
                  return t1->GetActiveTimePoint() > t2->GetActiveTimePoint();
              })
        {
        }
        ~TaskManager() = default;

        void PushTask() {}

    private:
        std::priority_queue<TaskPtr, std::vector<TaskPtr>, std::function<bool(TaskPtr&, TaskPtr&)>>
            m_taskqueue;
    };
}