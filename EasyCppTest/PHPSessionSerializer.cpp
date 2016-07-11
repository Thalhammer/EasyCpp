#include <gtest/gtest.h>
#include <StringAlgorithm.h>
#include <Serialize/PHPSessionSerializer.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(PHPSessionSerializer, Serialize)
	{
		Bundle b({
			{ "bool", true },
			{ "string", "Hallo" },
			{ "array", Bundle({ { "0", std::string("hallo") } }) },
			{ "null", nullptr },
			{ "double", 10.0 }
		});
		std::string res = Serialize::PHPSessionSerializer().serialize(b);
		ASSERT_EQ("array|a:1:{s:1:\"0\";s:5:\"hallo\";}bool|b:1;double|d:10.000000;null|N;string|s:5:\"Hallo\";", res);
	}

	TEST(PHPSessionSerializer, Deserialize)
	{
		std::string sessiondata = "bool|b:0;string|s:5:\"Hallo\";classobject|O:8:\"stdClass\":1:{s:8:\"property\";s:5:\"value\";}array|a:1:{i:0;s:5:\"hallo\";}null|N;integer|i:1;double|d:10;";
		Bundle res = Serialize::PHPSessionSerializer().deserialize(sessiondata).as<Bundle>();
		ASSERT_TRUE(res.isSet("bool"));
		ASSERT_TRUE(res.isSet("string"));
		ASSERT_TRUE(res.isSet("classobject"));
		ASSERT_TRUE(res.isSet("array"));
		ASSERT_TRUE(res.isSet("null"));
		ASSERT_TRUE(res.isSet("integer"));
		ASSERT_TRUE(res.isSet("double"));

		ASSERT_TRUE(res["bool"].isType<bool>());
		ASSERT_TRUE(res["string"].isType<std::string>());
		ASSERT_TRUE(res["classobject"].isType<Bundle>());
		ASSERT_TRUE(res["array"].isType<Bundle>());
		ASSERT_TRUE(res["null"].isType<nullptr_t>());
		ASSERT_TRUE(res["integer"].isType<int64_t>());
		ASSERT_TRUE(res["double"].isType<double>());

		ASSERT_EQ(false, res["bool"].as<bool>());
		ASSERT_EQ("Hallo", res["string"].as<std::string>());
		ASSERT_EQ(1, res["integer"].as<int64_t>());
		ASSERT_EQ(10, res["double"].as<double>());
		{
			auto c = res["classobject"].as<Bundle>();
			ASSERT_TRUE(c.isSet("property"));
			ASSERT_TRUE(c["property"].isType<std::string>());
			ASSERT_EQ("value", c["property"].as<std::string>());
		}
		{
			auto c = res["array"].as<Bundle>();
			ASSERT_TRUE(c.isSet("0"));
			ASSERT_TRUE(c["0"].isType<std::string>());
			ASSERT_EQ("hallo", c["0"].as<std::string>());
		}
	}
}