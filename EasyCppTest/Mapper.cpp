#include <gtest/gtest.h>
#include <Database/DatabaseDriver.h>
#include <Database/DatabaseDriverManager.h>
#include <Database/Mapper.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	class TestSerializable: public Serialize::Serializable
	{
	public:
		size_t id;
		std::string value;

		// Geerbt über Serializable
		virtual AnyValue toAnyValue() const override
		{
			return Bundle({
				{ "id", id },
				{ "value", value }
			});
		}
		virtual void fromAnyValue(const AnyValue & state) override
		{
			Bundle b = state.as<Bundle>();
			id = b.get<size_t>("id");
			value = b.get<std::string>("value");
		}
	};

	TEST(Mapper, BasicTest)
	{
		auto db = Database::DatabaseDriverManager::getDriver("sqlite3")->createInstance(":memory:");
		db->exec("CREATE TABLE \"test\" (\"id\" INTEGER PRIMARY KEY, \"value\" TEXT );");
		Database::Mapper mapper(db);
		mapper.setTypeTable(typeid(TestSerializable), "test");

		TestSerializable test;
		test.id = 100;
		test.value = "Hallo";

		mapper.store(test);

		auto test2 = mapper.read<TestSerializable>("WHERE `id` = ?", { 100 });

		ASSERT_EQ(test.id, test2.id);
		ASSERT_EQ(test.value, test2.value);
	}
}