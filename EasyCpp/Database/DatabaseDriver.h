#pragma once
#include "Database.h"
#include <vector>

namespace EasyCpp
{
	namespace Database
	{
		class DLL_EXPORT DatabaseDriver
		{
		public:
			virtual ~DatabaseDriver() {}
			/// <summary>Create a instance of this database type using the provided DSN and options.</summary>
			virtual DatabasePtr createInstance(const std::string& dsn, const Bundle& options = {}) = 0;
		};
		typedef std::shared_ptr<DatabaseDriver> DatabaseDriverPtr;
	}
}