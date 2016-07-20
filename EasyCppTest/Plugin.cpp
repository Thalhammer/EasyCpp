#include <gtest/gtest.h>
#include <Plugin/Manager.h>
#include <Plugin/IPluginDatabaseProvider.h>
#include <Database/DatabaseDriverManager.h>

using namespace EasyCpp::Plugin;

namespace EasyCppTest
{
	TEST(Plugin, LoadMySQLPlugin)
	{
		Manager mgr;
		mgr.loadPlugin("Mysql", "EasyCpp-Mysql.dll");

		auto provider = mgr.getInterface<IPluginDatabaseProvider>("Mysql");
		for (auto& i : provider->getDriverMap())
		{
			EasyCpp::Database::DatabaseDriverManager::registerDriver(i.first, i.second);
		}

		bool exception = false;
		try
		{
			// Try to use Database plugin
			auto dbdriver = EasyCpp::Database::DatabaseDriverManager::getDriver("mysql");
			auto db = dbdriver->createInstance("mysql:host=localhost;dbname=test", EasyCpp::Bundle({
				{"user", "root"},
				{"pass", "secret"}
			}));

			auto stmt = db->prepare("SELECT * FROM `test_table`");

			for (auto& i : provider->getDriverMap())
			{
				EasyCpp::Database::DatabaseDriverManager::deregisterDriver(i.first);
			}
		}
		catch (std::exception& e) {
			for (auto& i : provider->getDriverMap())
			{
				EasyCpp::Database::DatabaseDriverManager::deregisterDriver(i.first);
			}
			exception = true;
		}

		provider.reset();
		mgr.unloadPlugin("Mysql");

		ASSERT_FALSE(exception);
	}
}