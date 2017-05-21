#pragma once
#include <functional>
#include <memory>
#include <queue>
#include "types.h"

namespace terra
{
    class ScheduleTask
    {
		DISABLE_COPY(ScheduleTask)
	private:
		int64_t time_point_{ 0 };
	public:
		int64_t trigger_timepoint() const { return 0; }


	};

    class ScheduleTaskQueue
    {
        DISABLE_COPY(ScheduleTaskQueue);
        MAKE_INSTANCE(ScheduleTaskQueue);
        using TaskPtr = std::unique_ptr<ScheduleTask>;

    private:
        using TaskComparator = std::function<bool(TaskPtr&, TaskPtr&)>;
        std::priority_queue<TaskPtr, std::vector<TaskPtr>, TaskComparator> taskQueue_;

    public:
        ScheduleTaskQueue()
            : taskQueue_([](TaskPtr& lhs, TaskPtr& rhs) {
                  return rhs->trigger_timepoint() < lhs->trigger_timepoint();
              })
        {
        }
        ~ScheduleTaskQueue() = default;

        void PushTask() {}
    };
}