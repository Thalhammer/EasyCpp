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
		mgr.setAutoRegisterExtensions(true);
		mgr.loadPlugin("Mysql", "EasyCpp-Mysql.dll");

		bool exception = false;
		try
		{
			// Try to use Database plugin
			auto dbdriver = EasyCpp::Database::DatabaseDriverManager::getDriver("mysql");
		}
		catch (std::exception&) {
			exception = true;
		}
		mgr.deinitPlugin("Mysql");
		mgr.unloadPlugin("Mysql");

		ASSERT_FALSE(exception);
	}

	TEST(Plugin, DISABLED_LazyUnload)
	{
		bool exception = false;
		Manager mgr;
		mgr.setAutoRegisterExtensions(true);
		mgr.loadPlugin("Mysql", "EasyCpp-Mysql.dll");

		auto db = EasyCpp::Database::DatabaseDriverManager::getDriver("mysql");
		ASSERT_FALSE(mgr.canUnloadPlugin("Mysql"));
		try {
			mgr.unloadPlugin("Mysql");
		}
		catch (const std::exception&) { exception = true; }
		ASSERT_TRUE(exception);
		// Still a reference in "db"
		mgr.deinitPlugin("Mysql");
		ASSERT_FALSE(mgr.canUnloadPlugin("Mysql"));
		db.reset();
		ASSERT_TRUE(mgr.canUnloadPlugin("Mysql"));
		mgr.unloadPlugin("Mysql");
	}
}
