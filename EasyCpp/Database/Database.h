#pragma once
#include <string>
#include "../Bundle.h"

#include "Statement.h"

namespace EasyCpp
{
	namespace Database
	{
		class DLL_EXPORT Database
		{
		public:
			virtual ~Database() {}
			/// <summary>Starts a transaction if supported by this database, otherwise does nothing.</summary>
			virtual bool beginTransaction() = 0;
			/// <summary>Commit a transaction if supported by this database, otherwise does nothing.</summary>
			virtual bool commit() = 0;
			/// <summary>Rollback a transaction if supported by this database, otherwise does nothing.</summary>
			virtual bool rollBack() = 0;
			/// <summary>Checks if a transaction is currently running. This might return wrong results if a transaction is started using SQL ("START TRANSACTION;").</summary>
			virtual bool inTransaction() = 0;
			/// <summary>Returns the last error code as a string.</summary>
			virtual std::string errorCode() = 0;
			/// <summary>Returns additional error information.</summary>
			virtual Bundle errorInfo() = 0;
			/// <summary>Run SQL Statement and return number of rows affected.</summary>
			virtual uint64_t exec(const std::string& sql) = 0;
			/// <summary>Get a database specific attribute.</summary>
			virtual AnyValue getAttribute(const std::string& attribute) = 0;
			/// <summary>Set a database specific attribute.</summary>
			virtual bool setAttribute(const std::string& attribute, const AnyValue& value) = 0;
			/// <summary>Get the last inserted id of this connection.</summary>
			virtual std::string lastInsertedId(const std::string& name = "") = 0;
			/// <summary>Prepare a SQL statement.</summary>
			virtual StatementPtr prepare(const std::string& sql, const Bundle& driver_options = {}) = 0;
		};
		/// <summary>Shared pointer for a database connection.</summary>
		typedef std::shared_ptr<Database> DatabasePtr;
	}
}