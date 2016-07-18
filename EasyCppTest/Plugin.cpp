#include <gtest/gtest.h>
#include <Plugin/Manager.h>

using namespace EasyCpp::Plugin;

namespace EasyCppTest
{
	TEST(Plugin, LoadMySQLPlugin)
	{
		Manager mgr;
		mgr.loadPlugin("Mysql", "EasyCpp-Mysql.dll");

		mgr.unloadPlugin("Mysql");
	}
}