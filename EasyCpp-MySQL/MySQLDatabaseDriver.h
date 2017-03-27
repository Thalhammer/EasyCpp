#pragma once
#include <Database/DatabaseDriver.h>

namespace EasyCppMySql
{
	class MySQLDatabaseDriver : public EasyCpp::Database::DatabaseDriver
	{
	public:
		MySQLDatabaseDriver(std::shared_ptr<void> unloadp);
		virtual ~MySQLDatabaseDriver();
		virtual EasyCpp::Database::DatabasePtr createInstance(const std::string& dsn, const EasyCpp::Bundle& options = {}) override;
	private:
		std::shared_ptr<void> _unloadp;
	};
}