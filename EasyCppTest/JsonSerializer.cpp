#include <gtest/gtest.h>
#include <Serialize/JsonSerializer.h>

using namespace EasyCpp::Serialize;
using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(JsonSerializer, Object2Bundle)
	{
		JsonSerializer sjson;
		std::string str = "{\"hello\":\"world\"}";
		Bundle b = sjson.deserialize(str).as<Bundle>();

		ASSERT_TRUE(b.isSet("hello"));
		ASSERT_TRUE(b["hello"].isType<std::string>());
		ASSERT_EQ(b["hello"].as<std::string>(), "world");
	}

	TEST(JsonSerializer, Array2Bundle)
	{
		JsonSerializer sjson;
		std::string str = "[\"hello\",\"world\"]";
		auto b = sjson.deserialize(str).as<std::vector<AnyValue>>();

		ASSERT_EQ(2, b.size());
		ASSERT_EQ(b[0].as<std::string>(), "hello");
		ASSERT_EQ(b[1].as<std::string>(), "world");
	}
}
