#include "PluginDatabaseProvider.h"
#include "MySQLDatabaseDriver.h"

using namespace EasyCpp::Database;

namespace EasyCppMySql
{
	PluginDatabaseProvider::PluginDatabaseProvider()
	{
		_driver_map = std::map<std::string, DatabaseDriverPtr>({
			{ "mysql", std::make_shared<MySQLDatabaseDriver>() }
		});
	}
	PluginDatabaseProvider::~PluginDatabaseProvider()
	{
		
	}
	std::map<std::string, DatabaseDriverPtr> PluginDatabaseProvider::getDriverMap()
	{
		return _driver_map;
	}
}