#include <gtest/gtest.h>
#include <Hash/HOTP.h>

using namespace EasyCpp::Hash;

namespace EasyCppTest
{
	TEST(OTP, HOTP)
	{
		std::string secret = "3132333435363738393031323334353637383930";
		auto hotp = HOTP(secret);

		std::string token6 = hotp.generate(0, 6);
		std::string token8 = hotp.generate(0, 8);
		ASSERT_EQ(std::string("755224"), token6);
		ASSERT_EQ(std::string("84755224"), token8);

		token6 = hotp.generate(1, 6);
		token8 = hotp.generate(1, 8);
		ASSERT_EQ(std::string("287082"), token6);
		ASSERT_EQ(std::string("94287082"), token8);
	}
}