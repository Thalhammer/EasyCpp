#pragma once
#include <chrono>
#include <functional>

namespace EasyCpp
{
	template<typename Func = std::function<void(int64_t)>, typename Clock = std::chrono::high_resolution_clock, typename DurationType = std::chrono::milliseconds>
	class PerformanceCheck
	{
	public:
		PerformanceCheck(Func fn)
			:_done(false), _fn(std::move(fn))
		{
			_start = Clock::now();
		}
		~PerformanceCheck() {
			if (!_done)
				this->done();
		}
		void done() {
			_done = true;
			auto end = Clock::now();
			auto ts = end - _start;
			auto dur = std::chrono::duration_cast<DurationType>(ts);
			_fn(dur.count());
		}
	private:
		std::chrono::high_resolution_clock::time_point _start;
		bool _done;
		Func _fn;
	};

	template<typename DurationType = std::chrono::milliseconds, typename Clock = std::chrono::high_resolution_clock, typename Func>
	PerformanceCheck<Func, Clock, DurationType> make_performance_check(Func f)
	{
		return PerformanceCheck<Func, Clock, DurationType>(f);
	}
}