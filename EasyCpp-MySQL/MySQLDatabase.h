#pragma once
#include "MySQLHandle.h"
#include <Database/Database.h>

namespace EasyCppMySql
{
	class MySQLDatabase : public EasyCpp::Database::Database
	{
	public:
		MySQLDatabase(const std::string& dsn, const EasyCpp::Bundle& options, std::shared_ptr<void> unloadp);
		virtual ~MySQLDatabase();

		// Geerbt über Database
		virtual bool beginTransaction() override;
		virtual bool commit() override;
		virtual bool rollBack() override;
		virtual bool inTransaction() override;
		virtual std::string errorCode() override;
		virtual EasyCpp::Bundle errorInfo() override;
		virtual uint64_t exec(const std::string & sql) override;
		virtual EasyCpp::AnyValue getAttribute(const std::string & attribute) override;
		virtual bool setAttribute(const std::string & attribute, const EasyCpp::AnyValue & value) override;
		virtual std::string lastInsertedId(const std::string & name = "") override;
		virtual EasyCpp::Database::StatementPtr prepare(const std::string & sql, const EasyCpp::Bundle & driver_options = {}) override;
	private:
		std::shared_ptr<MySQLHandle> _hdl;
		std::shared_ptr<void> _unloadp;
	};
}