#pragma once
#include <Database/DatabaseDriver.h>
#include "RefCounted.h"

namespace EasyCppMySql
{
	class MySQLDatabaseDriver : public EasyCpp::Database::DatabaseDriver, RefCounted
	{
	public:
		MySQLDatabaseDriver();
		virtual ~MySQLDatabaseDriver();
		virtual EasyCpp::Database::DatabasePtr createInstance(const std::string& dsn, const EasyCpp::Bundle& options = {}) override;
	private:
	};
}