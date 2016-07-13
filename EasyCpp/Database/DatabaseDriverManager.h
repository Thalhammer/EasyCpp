#pragma once
#include "DatabaseDriver.h"
#include <vector>

namespace EasyCpp
{
	namespace Database
	{
		class DLL_EXPORT DatabaseDriverManager
		{
		public:
			static std::vector<std::string> getAvailableDrivers();
			static void registerDriver(const std::string& name, DatabaseDriverPtr driver);
			static DatabaseDriverPtr getDriver(const std::string& name);
		private:
			DatabaseDriverManager();
			static std::map<std::string, DatabaseDriverPtr> _drivers;
		};
	}
}