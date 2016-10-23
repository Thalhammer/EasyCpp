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
		mgr.unloadPlugin("Mysql");

		ASSERT_FALSE(exception);
	}
}
