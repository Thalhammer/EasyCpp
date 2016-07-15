#include <gtest/gtest.h>
#include <VarArgs.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(VarArgs, DynamicCall)
	{
		std::function<int(int, int)> fn = [](int i, int i2) {
			return i + i2;
		};

		AnyArray args = { 10, 100 };
		AnyValue val = VarArgs::call(fn, args);
		ASSERT_TRUE(val.isType<int>());
		ASSERT_EQ(110, val.as<int>());
	}
}
