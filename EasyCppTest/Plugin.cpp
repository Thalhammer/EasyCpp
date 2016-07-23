#include <gtest/gtest.h>
#include <Plugin/Manager.h>
#include <Plugin/IPluginDatabaseProvider.h>
#include <Database/DatabaseDriverManager.h>

using namespace EasyCpp::Plugin;

namespace EasyCppTest
{
	TEST(Plugin, DISABLED_LoadMySQLPlugin)
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
				{"user", "unittest"},
				{"pass", "secret"}
			}));

			auto stmt = db->prepare("SELECT COUNT(*) FROM `test`");
			auto val = stmt->executeScalar();

			for (auto& i : provider->getDriverMap())
			{
				EasyCpp::Database::DatabaseDriverManager::deregisterDriver(i.first);
			}
		}
		catch (std::exception&) {
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
