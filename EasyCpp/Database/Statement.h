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

			virtual uint64_t execute() = 0;
			virtual AnyValue executeScalar() = 0;
			virtual Bundle executeQueryRow() = 0;
			virtual ResultSet executeQuery() = 0;

			virtual void bind(uint64_t id, AnyValue value) = 0;
			virtual void bind(const std::string& id, AnyValue value) = 0;
		};
		typedef std::shared_ptr<Statement> StatementPtr;
	}
}