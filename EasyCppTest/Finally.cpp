#include <gtest/gtest.h>
#include <Finally.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Finally, FunctionExecutedOnScopeLeave)
	{
		bool executed = false;
		{
			Finally finally([&executed]() {
				executed = true;
			});
		}
		ASSERT_TRUE(executed);
	}

	TEST(Finally, CanBeDismissed)
	{
		bool executed = false;
		{
			Finally finally([&executed]() {
				executed = true;
			});
			finally.dismiss();
		}
		ASSERT_FALSE(executed);
	}
}