#include <gtest/gtest.h>
#include <ValueConverter.h>
#include <ConvertException.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Convert, BoolToString)
	{
		ASSERT_EQ(std::string("true"), (ValueConverter::convert<bool, std::string>(true)));
		ASSERT_EQ(std::string("false"), (ValueConverter::convert<bool, std::string>(false)));
	}

	TEST(Convert, StringToBool)
	{
		ASSERT_EQ(true, (ValueConverter::convert<std::string, bool>("true")));
		ASSERT_EQ(false, (ValueConverter::convert<std::string, bool>("false")));
		ASSERT_THROW({
			(ValueConverter::convert<std::string, bool>("invalid"));
		}, ConvertException);
	}

	TEST(Convert, Integer)
	{
		ASSERT_EQ(std::string("10"), (ValueConverter::convert<uint16_t, std::string>(10)));
		ASSERT_EQ(std::string("10"), (ValueConverter::convert<unsigned int, std::string>(10)));
		ASSERT_EQ(10, (ValueConverter::convert<std::string, int>("10")));
		ASSERT_THROW({
			(ValueConverter::convert<std::string, int8_t>("130"));
		}, ConvertException);
	}
}