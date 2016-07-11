#include <gtest/gtest.h>
#include <Serialize/BsonSerializer.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(BsonSerializer, BuildDocument)
	{
		Bundle b;
		b.set("hello", "world");
		Serialize::BsonSerializer bson;
		auto result = bson.serialize(b);
		ASSERT_EQ(result.size(), 22);
		ASSERT_EQ(result, std::string({ 0x16,0x00,0x00,0x00,0x02,'h','e','l','l','o',0x00,0x06,0x00,0x00,0x00,'w','o','r','l','d',0x00,0x00 }));
	}

	TEST(BsonSerializer, ReadDocument)
	{
		std::string doc({ 0x16,0x00,0x00,0x00,0x02,'h','e','l','l','o',0x00,0x06,0x00,0x00,0x00,'w','o','r','l','d',0x00,0x00 });
		Serialize::BsonSerializer bson;
		auto b = bson.deserialize(doc).as<Bundle>();
		ASSERT_FALSE(b.isEmpty());
		ASSERT_TRUE(b.isSet("hello"));
		ASSERT_TRUE(b.get("hello").isType<std::string>());
		ASSERT_EQ(b.get<std::string>("hello"), std::string("world"));
	}
}