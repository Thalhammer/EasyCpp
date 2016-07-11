#pragma once
#include <chrono>
#include <functional>

namespace Util
{
	class PerformanceCheck
	{
	public:
		PerformanceCheck(std::string id, std::function<void(const std::string&, int64_t)> fn)
			:_done(false), _id(id), _fn(fn)
		{
			_start = std::chrono::high_resolution_clock::now();
		}
		~PerformanceCheck() {
			if (!_done)
				this->done();
		}
		void done() {
			_done = true;
			auto end = std::chrono::high_resolution_clock::now();
			auto ts = end - _start;
			auto dur = std::chrono::duration_cast<std::chrono::milliseconds>(ts);
			_fn(_id, dur.count());
		}
	private:
		std::chrono::high_resolution_clock::time_point _start;
		bool _done;
		std::string _id;
		std::function<void(const std::string&, int64_t)> _fn;
	};
}