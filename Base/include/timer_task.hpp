#pragma once
#include <functional>
#include <memory>
#include <queue>
#include "base_macro.h"
namespace TerraX
{
    class TimerTask
    {
        DISABLE_COPY(TimerTask);

    public:
        explicit TimerTask(int nTimePoint) : m_nActiveTimePoint(nTimePoint) {}
		~TimerTask() {}
        int GetActiveTimePoint() const { return m_nActiveTimePoint; }

    private:
        int m_nActiveTimePoint{0};
    };

    class TaskList
    {
        DISABLE_COPY(TaskList);
        MAKE_INSTANCE(TaskList);
        using TaskPtr = std::unique_ptr<TimerTask>;
    public:
        TaskList()
            : m_taskqueue([](TaskPtr& lhs, TaskPtr& rhs) {
                  return rhs->GetActiveTimePoint() < lhs->GetActiveTimePoint();
              })
        {
        }
        ~TaskList() = default;

        void PushTask() {}

	private:
		using TaskComparator = std::function<bool(TaskPtr&, TaskPtr&)>;
        std::priority_queue<TaskPtr, std::vector<TaskPtr>, TaskComparator> m_taskqueue;
    };
}