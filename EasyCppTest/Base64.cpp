#include <gtest/gtest.h>
#include <Base64.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Base64, BinaryToString)
	{
		std::string res = Base64::toString("Hallo Welt");
		ASSERT_EQ(res, std::string("SGFsbG8gV2VsdA=="));
	}

	TEST(Base64, StringToBinary)
	{
		std::string base = "SGFsbG8gV2VsdA==";
		std::vector<uint8_t> res = Base64::toBinary(base);
		std::string res_str(res.begin(), res.end());
		ASSERT_EQ(res_str, std::string("Hallo Welt"));
	}
}