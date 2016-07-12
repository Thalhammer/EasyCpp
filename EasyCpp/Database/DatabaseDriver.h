#pragma once
#include "Database.h"
#include <vector>

namespace EasyCpp
{
	namespace Database
	{
		class DatabaseDriver;
		typedef std::shared_ptr<DatabaseDriver> DatabaseDriverPtr;
		class DatabaseDriver
		{
		public:
			virtual ~DatabaseDriver() {}
			virtual DatabasePtr createInstance(const std::string& dsn, const Bundle& options = {}) = 0;

			static std::vector<std::string> getAvailableDrivers();
			static void registerDriver(const std::string& name, DatabaseDriverPtr driver);
			static DatabaseDriverPtr getDriver(const std::string& name);
		private:
			static std::map<std::string, DatabaseDriverPtr> _drivers;
		};
	}
}