#include <gtest/gtest.h>
#include <HexEncoding.h>

namespace EasyCppTest
{
	TEST(HexEncoding, HexEncoding)
	{
		std::string str = std::string({ 0x01, 0x23, 0x45, 0x67, char(0x89), (char)0xab, (char)0xcd, (char)0xef });
		std::string hex = EasyCpp::HexEncoding::encode(str);
		ASSERT_EQ(std::string("0123456789abcdef"), hex);
		hex = EasyCpp::HexEncoding::decode(hex);
		ASSERT_EQ(str, hex);
	}
}