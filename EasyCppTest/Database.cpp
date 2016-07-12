#include <gtest/gtest.h>
#include <Database/DatabaseDriver.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(Database, SQLITEDatabaseCreate)
	{
		auto db = Database::DatabaseDriver::getDriver("sqlite3")->createInstance(":memory:");
		ASSERT_NE(db.get(), nullptr);
		db.reset();
		ASSERT_EQ(db.get(), nullptr);
	}

	TEST(Database, SQLITEStatementExecute)
	{
		auto db = Database::DatabaseDriver::getDriver("sqlite3")->createInstance(":memory:");
		ASSERT_NE(db.get(), nullptr);
		auto stmt = db->prepare("SELECT 1");
		ASSERT_NE(stmt.get(), nullptr);
		auto value = stmt->executeScalar();
		ASSERT_TRUE(value.isConvertibleTo<int64_t>());
		ASSERT_EQ(value.as<int64_t>(), 1);
	}

	TEST(Database, SQLITEInvalidStatement)
	{
		auto db = Database::DatabaseDriver::getDriver("sqlite3")->createInstance(":memory:");
		ASSERT_NE(db.get(), nullptr);
		ASSERT_THROW({
			db->exec("SELECT a FROM b");
		}, std::exception);
	}

	TEST(Database, SQLITEInsertSelect)
	{
		auto db = Database::DatabaseDriver::getDriver("sqlite3")->createInstance(":memory:");
		ASSERT_NE(db.get(), nullptr);
		db->exec("CREATE TABLE \"test\" (\"id\" INTEGER PRIMARY KEY AUTOINCREMENT, \"value\" TEXT );");
		auto stmt = db->prepare("SELECT * FROM sqlite_master WHERE tbl_name = 'test'; ");
		ASSERT_NE(stmt.get(), nullptr);
		auto res = stmt->executeQuery();
		ASSERT_EQ(res.getColumns().size(), 5);
		ASSERT_EQ(res.size(), 1);

		db->beginTransaction();
		stmt = db->prepare("INSERT INTO test (`value`) VALUES (?)");
		ASSERT_NE(stmt.get(), nullptr);
		for (int i = 0; i < 100; i++)
		{
			stmt->bind(0, (int64_t)i);
			int ret = stmt->execute();
			ASSERT_EQ(ret, 1);
		}
		db->commit();
		stmt = db->prepare("SELECT COUNT(*) FROM test");
		ASSERT_NE(stmt.get(), nullptr);
		ASSERT_EQ(stmt->executeScalar().as<int64_t>(), 100);
		res = db->prepare("SELECT * FROM test")->executeQuery();
		ASSERT_EQ(res.getColumns().size(), 2);
		ASSERT_EQ(res.size(), 100);
		for (int i = 0; i < 100; i++)
		{
			std::string row = res[i]["value"].as<std::string>();
			std::string soll = std::to_string(i);
			ASSERT_EQ(row, soll);
		}
	}
}