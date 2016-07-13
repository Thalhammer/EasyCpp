#pragma once
#include "Database.h"
#include <vector>

namespace EasyCpp
{
	namespace Database
	{
		class DatabaseDriver;
		typedef std::shared_ptr<DatabaseDriver> DatabaseDriverPtr;
		class DLL_EXPORT DatabaseDriver
		{
		public:
			virtual ~DatabaseDriver() {}
			virtual DatabasePtr createInstance(const std::string& dsn, const Bundle& options = {}) = 0;
		};
	}
}