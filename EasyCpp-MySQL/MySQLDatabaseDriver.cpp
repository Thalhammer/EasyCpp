#include "MySQLDatabaseDriver.h"

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
		throw std::logic_error("Not yet implemented");
	}
}