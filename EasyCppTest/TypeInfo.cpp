#include <gtest/gtest.h>
#include <TypeInfo.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(TypeInfo, Int)
	{
		TypeInfo t = TypeInfo::CreateInfo<int>();
		ASSERT_FALSE(t.isAbstract());
		ASSERT_TRUE(t.isArithmetic());
		ASSERT_FALSE(t.isArray());
		ASSERT_FALSE(t.isClass());
	}
}