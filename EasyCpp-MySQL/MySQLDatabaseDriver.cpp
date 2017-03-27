#include "MySQLDatabaseDriver.h"
#include "MySQLDatabase.h"

namespace EasyCppMySql
{

	MySQLDatabaseDriver::MySQLDatabaseDriver(std::shared_ptr<void> unloadp)
		:_unloadp(unloadp)
	{
	}

	MySQLDatabaseDriver::~MySQLDatabaseDriver()
	{
	}

	EasyCpp::Database::DatabasePtr MySQLDatabaseDriver::createInstance(const std::string & dsn, const EasyCpp::Bundle & options)
	{
		return std::make_shared<MySQLDatabase>(dsn, options, _unloadp);
	}
}