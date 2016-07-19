#include "MySQLDatabaseDriver.h"
#include "MySQLDatabase.h"

namespace EasyCppMySql
{

	MySQLDatabaseDriver::MySQLDatabaseDriver()
	{
	}

	MySQLDatabaseDriver::~MySQLDatabaseDriver()
	{
	}

	EasyCpp::Database::DatabasePtr MySQLDatabaseDriver::createInstance(const std::string & dsn, const EasyCpp::Bundle & options)
	{
		return std::make_shared<MySQLDatabase>(dsn, options);
	}
}