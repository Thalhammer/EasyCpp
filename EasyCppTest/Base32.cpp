#include <gtest/gtest.h>
#include <Base32.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Base32, BinaryToString)
	{
		std::string res = Base32::toString("Hallo Welt");
		ASSERT_EQ(res, std::string("JBQWY3DPEBLWK3DU"));
	}

	TEST(Base32, StringToBinary)
	{
		std::string base = "JBQWY3DPEBLWK3DU";
		std::vector<uint8_t> res = Base32::toBinary(base);
		std::string res_str(res.begin(), res.end());
		ASSERT_EQ(res_str, std::string("Hallo Welt"));
	}
}