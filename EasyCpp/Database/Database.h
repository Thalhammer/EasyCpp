#pragma once
#include <string>
#include "../Bundle.h"

#include "Statement.h"

namespace EasyCpp
{
	namespace Database
	{
		class Database
		{
		public:
			virtual ~Database() {}
			virtual bool beginTransaction() = 0;
			virtual bool commit() = 0;
			virtual bool rollBack() = 0;
			virtual bool inTransaction() = 0;
			virtual std::string errorCode() = 0;
			virtual Bundle errorInfo() = 0;
			virtual uint64_t exec(const std::string& sql) = 0;
			virtual AnyValue getAttribute(const std::string& attribute) = 0;
			virtual bool setAttribute(const std::string& attribute, const AnyValue& value) = 0;
			virtual std::string lastInsertedId(const std::string& name = "") = 0;

			virtual StatementPtr prepare(const std::string& sql, const Bundle& driver_options = {}) = 0;
		};
		typedef std::shared_ptr<Database> DatabasePtr;
	}
}