#include <gtest/gtest.h>
#include <Serialize/Vector.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(SerializeVector, Int)
	{
		Serialize::Vector<int> vect;
		vect.push_back(10);
		vect.push_back(100);

		auto any = vect.toAnyValue();
		ASSERT_TRUE(any.isType<std::vector<AnyValue>>());
		auto as = any.as<std::vector<AnyValue>>();
		ASSERT_EQ(2, as.size());
		ASSERT_TRUE(as[0].isType<int>());
		ASSERT_TRUE(as[1].isType<int>());
		ASSERT_EQ(10, as[0].as<int>());
		ASSERT_EQ(100, as[1].as<int>());
	}
}