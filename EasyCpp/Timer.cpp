#include "Timer.h"

namespace EasyCpp
{
	Timer::SimpleTask::SimpleTask(std::chrono::steady_clock::time_point tp, std::function<void()> fn)
		: _tp(tp), _fn(fn)
	{
	}

	Timer::SimpleTask::~SimpleTask()
	{
	}

	std::chrono::steady_clock::time_point Timer::SimpleTask::time_point() const
	{
		return _tp;
	}

	void Timer::SimpleTask::next_time_point()
	{
	}

	bool Timer::SimpleTask::schouldReschedule() const
	{
		return false;
	}

	void Timer::SimpleTask::execute()
	{
		_fn();
	}

	Timer::RecurringTask::RecurringTask(std::chrono::steady_clock::time_point start_tp, std::chrono::steady_clock::duration dur, std::function<void()> fn)
		: _tp(start_tp), _dur(dur), _fn(fn)
	{
	}

	Timer::RecurringTask::~RecurringTask()
	{
	}

	std::chrono::steady_clock::time_point Timer::RecurringTask::time_point() const
	{
		return _tp;
	}

	void Timer::RecurringTask::next_time_point()
	{
		_tp = _tp + _dur;
	}

	bool Timer::RecurringTask::schouldReschedule() const
	{
		return true;
	}

	void Timer::RecurringTask::execute()
	{
		_fn();
	}
	Timer::Timer()
	{
		_thread_exit.store(false);
		_thread = std::thread([this]() {
			while (!_thread_exit.load()) {
				try {
					std::unique_lock<std::mutex> lck(_mtx);
					if (_tasks.empty()) {
						_cv.wait(lck);
					}
					else {
						if (std::chrono::steady_clock::now() >= _tasks.top().time_point()) {
							auto task = _tasks.top();
							_tasks.pop();
							lck.unlock();
							task.execute();
							if (task.schouldReschedule()) {
								task.next_time_point();
								std::unique_lock<std::mutex> lck(_mtx);
								_tasks.push(task);
							}
						}
						else {
							_cv.wait_until(lck, _tasks.top().time_point());
						}
					}
				}catch(...) {
					std::unique_lock<std::mutex> lck(_mtx);
					_on_exception(std::current_exception());
				}
			}
		});
		setExceptionHandler([](auto ex) {
			std::rethrow_exception(ex);
		});
	}

	Timer::~Timer()
	{
		_thread_exit.store(true);
		{
			std::unique_lock<std::mutex> lck(_mtx);
			_cv.notify_all();
		}
		_thread.join();
	}

	void Timer::setExceptionHandler(std::function<void(std::exception_ptr)> fn)
	{
		std::unique_lock<std::mutex> lck(_mtx);
		_on_exception = fn;
	}

	Timer::TaskPtr Timer::schedule(std::chrono::steady_clock::time_point tp, std::function<void()> fn)
	{
		return this->schedule(std::make_shared<SimpleTask>(tp, fn));
	}

	Timer::TaskPtr Timer::schedule(TaskPtr task)
	{
		std::unique_lock<std::mutex> lck(_mtx);
		_tasks.push(task);
		_cv.notify_all();
		return task;
	}
}