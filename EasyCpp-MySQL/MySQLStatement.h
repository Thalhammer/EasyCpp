#pragma once
#include "RefCounted.h"
#include "MySQLHandle.h"
#include <Database/Statement.h>

namespace EasyCppMySql
{
	class MySQLStatement : public EasyCpp::Database::Statement, RefCounted
	{
	public:
		MySQLStatement(const std::string& sql, std::shared_ptr<MySQLHandle> hdl);
		~MySQLStatement();

		MySQLStatement(MySQLStatement const&) = delete;
		void operator=(MySQLStatement const&) = delete;

		// Geerbt über Statement
		virtual uint64_t execute() override;
		virtual EasyCpp::AnyValue executeScalar() override;
		virtual EasyCpp::Bundle executeQueryRow() override;
		virtual EasyCpp::Database::ResultSet executeQuery() override;
		virtual void bind(uint64_t id, EasyCpp::AnyValue value) override;
		virtual void bind(const std::string & id, EasyCpp::AnyValue value) override;
	private:
		void setBind(uint64_t idx, enum_field_types type, void* data, unsigned long dlen, bool sign);

		static EasyCpp::AnyValue bind2Result(MYSQL_BIND* bind);
		static enum_field_types convertType(enum_field_types t);
		static std::pair<std::vector<std::string>, std::unique_ptr<MYSQL_BIND, std::function<void(MYSQL_BIND*)>>> getBind(MYSQL_RES* res);

		MYSQL_STMT* _stmt;

		MYSQL_BIND* _param_bind;
		unsigned long _param_count;

		std::shared_ptr<MySQLHandle> _hdl;
	};
}