#pragma once
#include <chrono>
#include <functional>
#include <queue>
#include <thread>
#include <chrono>
#include <mutex>
#include <atomic>
#include <condition_variable>
#include <memory>
#include <stdexcept>
#include "DllExport.h"

namespace EasyCpp
{
	class DLL_EXPORT Timer
	{
	public:
		class DLL_EXPORT Task
		{
		public:
			virtual ~Task() {}
			virtual std::chrono::steady_clock::time_point time_point() const = 0;
			virtual void next_time_point() = 0;
			virtual bool schouldReschedule() const = 0;
			virtual void execute() = 0;
		};
		typedef std::shared_ptr<Task> TaskPtr;
		class DLL_EXPORT SimpleTask: public Task
		{
		public:
			SimpleTask(std::chrono::steady_clock::time_point tp, std::function<void()> fn);
			virtual ~SimpleTask();
			// Geerbt via Task
			virtual std::chrono::steady_clock::time_point time_point() const override;
			virtual void next_time_point() override;
			virtual bool schouldReschedule() const override;
			virtual void execute() override;
		private:
			std::function<void()> _fn;
			std::chrono::steady_clock::time_point _tp;
		};
		class DLL_EXPORT RecurringTask : public Task
		{
		public:
			RecurringTask(std::chrono::steady_clock::time_point start_tp, std::chrono::steady_clock::duration dur, std::function<void()> fn);
			virtual ~RecurringTask();
			// Geerbt via Task
			virtual std::chrono::steady_clock::time_point time_point() const override;
			virtual void next_time_point() override;
			virtual bool schouldReschedule() const override;
			virtual void execute() override;
		private:
			std::function<void()> _fn;
			std::chrono::steady_clock::time_point _tp;
			std::chrono::steady_clock::duration _dur;
		};
	private:
		class TaskWrapper
		{
		public:
			TaskWrapper(TaskPtr task)
				: _task(task)
			{}
			bool operator <(const TaskWrapper& rhs) const
			{
				return time_point() > rhs.time_point();
			}
			std::chrono::steady_clock::time_point time_point() const
			{
				return _task->time_point();
			}
			void next_time_point()
			{
				_task->next_time_point();
			}
			bool schouldReschedule() const
			{
				return _task->schouldReschedule();
			}
			void execute()
			{
				_task->execute();
			}
		private:
			TaskPtr _task;
		};

		std::priority_queue<TaskWrapper> _tasks;
		std::function<void(std::exception_ptr)> _on_exception;
		std::thread _thread;
		std::atomic<bool> _thread_exit;
		std::mutex _mtx;
		std::condition_variable _cv;
	public:
		Timer();
		virtual ~Timer();

		void setExceptionHandler(std::function<void(std::exception_ptr)> fn);

		TaskPtr schedule(std::chrono::steady_clock::time_point tp, std::function<void()> fn);
		TaskPtr schedule(TaskPtr task);

		template<typename Rep, typename Period = std::ratio<1>>
		TaskPtr schedule(std::chrono::duration<Rep, Period> dur, std::function<void()> fn, bool recuring = false)
		{
			if (!recuring) return this->schedule(std::chrono::steady_clock::now() + dur, fn);
			else {
				return this->schedule(std::make_shared<RecurringTask>(std::chrono::steady_clock::now(), std::chrono::duration_cast<std::chrono::steady_clock::duration>(dur), fn));
			}
		}

		template<typename Rep, typename Period = std::ratio<1>>
		TaskPtr schedule(std::chrono::steady_clock::time_point start_tp, std::chrono::duration<Rep, Period> dur, std::function<void()> fn, bool recuring = false)
		{
			if (!recuring) return this->schedule(start_tp + dur, fn);
			else {
				return this->schedule(std::make_shared<RecurringTask>(start_tp, std::chrono::duration<std::chrono::steady_clock::duration>(dur), fn));
			}
		}
	};
}