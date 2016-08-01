#include <gtest/gtest.h>
#include <Hash/HOTP.h>
#include <Hash/TOTP.h>
#include <HexEncoding.h>

using namespace EasyCpp;
using namespace EasyCpp::Hash;

namespace EasyCppTest
{
	TEST(OTP, HOTP)
	{
		std::string secret = "3132333435363738393031323334353637383930";
		auto hotp = HOTP(HexEncoding::decode(secret));

		std::string token6 = hotp.generate(0, 6);
		std::string token8 = hotp.generate(0, 8);
		ASSERT_EQ(std::string("755224"), token6);
		ASSERT_EQ(std::string("84755224"), token8);

		token6 = hotp.generate(1, 6);
		token8 = hotp.generate(1, 8);
		ASSERT_EQ(std::string("287082"), token6);
		ASSERT_EQ(std::string("94287082"), token8);
	}

	TEST(OTP, TOTP)
	{
		std::string secret = "3132333435363738393031323334353637383930";
		auto totp = TOTP(HexEncoding::decode(secret));

		std::vector<std::string> keys;
		for (int i = -5; i < 6; i++)
			keys.push_back(totp.generate(i));

		// Set a break point here and check using your TOTP device !
	}
}