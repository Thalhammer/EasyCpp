#include <gtest/gtest.h>
#include <PerformanceCheck.h>
#include <thread>
#include <chrono>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(PerformanceCheck, Performance)
	{
		bool called = false;
		{
			auto t = make_performance_check([&](int64_t time) {
				called = true;
				ASSERT_GT(time, 10);
			});
			std::this_thread::sleep_for(std::chrono::milliseconds(20));
		}
		ASSERT_TRUE(called);
	}
}