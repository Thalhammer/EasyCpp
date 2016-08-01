#pragma once
#include <string>
#include "../Bundle.h"
#include "ResultSet.h"

namespace EasyCpp
{
	namespace Database
	{
		class Statement
		{
		public:
			virtual ~Statement() {}

			/// <summary>Execute statement and return number of rows affected.</summary>
			virtual uint64_t execute() = 0;
			/// <summary>Execute statement and return first column of first row.
			/// Throws a exception if the statement does not return exactly one row containing one column.</summary>
			virtual AnyValue executeScalar() = 0;
			/// <summary>Execute statement and return first row.
			/// Throws a exception if the statement does not return exactly one row.</summary>
			virtual Bundle executeQueryRow() = 0;
			/// <summary>Execute statement and return its results.</summary>
			virtual ResultSet executeQuery() = 0;

			/// <summary>Bind a parameter by index.</summary>
			virtual void bind(uint64_t id, AnyValue value) = 0;
			/// <summary>Bind a parameter by name. Some databases might not support named parameters.</summary>
			virtual void bind(const std::string& id, AnyValue value) = 0;
		};
		/// <summary>Statement pointer</summary>
		typedef std::shared_ptr<Statement> StatementPtr;
	}
}