#include <gtest/gtest.h>
#include <Serialize/MinistoreSerializer.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(MinistoreSerializer, BuildDocument)
	{
		Bundle b;
		b.set("hello", "world");
		Serialize::MinistoreSerializer store;
		auto result = store.serialize(b);
		ASSERT_EQ(18, result.size());
		// Size               type  refid strl string             | backref            strend
		ASSERT_EQ(std::string({ 0x0c,0x00,0x00,0x00,0x26, 0x00, 0x05,'w','o','r','l','d','h','e','l','l','o',0x00 }), result);
	}

	TEST(MinistoreSerializer, BuildDocumentStacked)
	{
		Bundle b;
		b.set("hello", "world");
		Bundle b2;
		b2.set("test_value", "1234");
		b.set("obj", b2);
		Serialize::MinistoreSerializer store;
		auto result = store.serialize(b);
		ASSERT_EQ(46, result.size());
		ASSERT_EQ(std::string({ 0x19,0x00,0x00,0x00,0x26, 0x00, 0x05,'w','o','r','l','d',
			0x21, 0x01, 0x0B, 0x00, 0x00, 0x00, 0x26, 0x02, 0x04, '1',
			'2', '3', '4', 'h', 'e', 'l', 'l', 'o', 0x00, 'o', 'b',
			'j', 0x00, 't', 'e', 's', 't', '_', 'v', 'a', 'l', 'u',
			'e', 0x00 }), result);
	}

	TEST(MinistoreSerializer, BuildDocumentCodeTable)
	{
		Bundle b;
		b.set("hello", "world");
		Serialize::MinistoreSerializer store;
		// Setup codetable
		store.setCodeTable({
			"hello",
			"world"
		});
		store.setValueCodeTable(false);
		auto result = store.serialize(b);
		ASSERT_EQ(12, result.size());
		// Size                    type  refid strl string
		ASSERT_EQ(std::string({ 0x0c, 0x00, 0x00, 0x00, (char)0x96, 0x00, 0x05,'w','o','r','l','d' }), result);
	}

	TEST(MinistoreSerializer, BuildDocumentCodeTableFull)
	{
		Bundle b;
		b.set("hello", "world");
		Serialize::MinistoreSerializer store;
		// Setup codetable
		store.setCodeTable({
			"hello",
			"world"
		});
		store.setValueCodeTable(true);
		auto result = store.serialize(b);
		ASSERT_EQ(result.size(), 8);
		// Size                    type  refid strl string
		ASSERT_EQ(std::string({ 0x08, 0x00, 0x00, 0x00, (char)0x9A, 0x00, 0x01, (char)0x80 }), result);
	}
}