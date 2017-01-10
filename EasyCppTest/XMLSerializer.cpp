#include <gtest/gtest.h>
#include <Serialize/XMLSerializer.h>
#include <Bundle.h>
#include <AnyArray.h>

using namespace EasyCpp::Serialize;
using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(XMLSerializer, SerializeDeserialize)
	{
		XMLSerializer xml;
		EasyCpp::Bundle data({
			{"elem_test", "elemdata"},
			{"-att_id", "attributedata"},
			{"subelem", EasyCpp::Bundle({
				{"test", "test3"},
				{"sample2", "sample"}
			})},
			{"elem_array", EasyCpp::AnyArray({
				"Test",
				"Test2",
				"Test3"
			})}
		});
		std::string res = xml.serialize(data);

		EasyCpp::AnyValue val = xml.deserialize(res);
		ASSERT_TRUE(val.isType<EasyCpp::Bundle>());
		auto bundle = val.as<EasyCpp::Bundle>();
		ASSERT_TRUE(bundle.isSet("elem_test"));
		ASSERT_TRUE(bundle.isSet("-att_id"));
		ASSERT_TRUE(bundle.isSet("subelem"));
		ASSERT_TRUE(bundle.isSet("elem_array"));
		auto anyarray = bundle.get("elem_array");
		ASSERT_TRUE(anyarray.isType<EasyCpp::AnyArray>());
		auto array = anyarray.as<EasyCpp::AnyArray>();
		ASSERT_EQ(3, array.size());
		ASSERT_EQ("Test", array[0].as<std::string>());
	}
}
