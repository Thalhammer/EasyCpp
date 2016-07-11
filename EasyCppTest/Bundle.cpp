#include <gtest/gtest.h>
#include <Bundle.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Bundle, StoreReadValue)
	{
		Bundle b;
		b.set("test", 10);
		ASSERT_EQ(int16_t(10), b.get<int16_t>("test"));
	}
	TEST(Bundle, UnsetKeyIsNullPTR)
	{
		Bundle b;
		ASSERT_TRUE(b.get("unset").isType<nullptr_t>());
	}

	TEST(Bundle, IsSet)
	{
		Bundle b;
		b.set("test", AnyValue(10));
		ASSERT_TRUE(b.isSet("test"));
		ASSERT_FALSE(b.isSet("unset"));
	}

	TEST(Bundle, MapAccess)
	{
		Bundle b;
		b["test"] = (int)10;
		ASSERT_TRUE(b.isSet("test"));
		ASSERT_TRUE(b["test"].isType<int>());
		ASSERT_EQ(10, b["test"].as<int>());
	}
}