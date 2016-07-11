#include <gtest/gtest.h>
#include <StringAlgorithm.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(StringAlgorithm, toLower)
	{
		std::string str = "hElLo WoRld";
		ASSERT_EQ("hello world", stringToLower(str));
	}

	TEST(StringAlgorithm, split)
	{
		std::string str = "Hello;World;!";
		auto res = stringSplit(";", str);
		ASSERT_EQ(3, res.size());
		ASSERT_EQ("Hello", res[0]);
		ASSERT_EQ("World", res[1]);
		ASSERT_EQ("!", res[2]);
	}
}