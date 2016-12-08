#include <gtest/gtest.h>
#include <Hash/HMAC.h>
#include <HexEncoding.h>

using namespace EasyCpp::Hash;

namespace EasyCppTest
{
	TEST(HMAC, Run)
	{
		std::string message = "Hi There";
		std::string key = std::string(16, 0x0b);

		std::string hmac = EasyCpp::HexEncoding::encode(HMAC("md5").run(message, key));

		ASSERT_EQ(std::string("9294727a3638bb1c13f48ef8158bfc9d"), hmac);
	}
}