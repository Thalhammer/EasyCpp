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
		void setBind(unsigned long idx, enum_field_types type, void* data, unsigned long dlen, bool sign);

		static enum_field_types convertType(enum_field_types t);

		MYSQL_STMT* _stmt;

		MYSQL_BIND* _param_bind;
		unsigned long _param_count;
		MYSQL_BIND* _result_meta_bind;
		unsigned long _result_meta_count;
		std::vector<std::string> _result_names;

		std::shared_ptr<MySQLHandle> _hdl;
	};
}