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
			/// <summary>Get a vector of all available driver names.</summary>
			static std::vector<std::string> getAvailableDrivers();
			/// <summary>Register a driver.</summary>
			static void registerDriver(const std::string& name, DatabaseDriverPtr driver);
			/// <summary>Remove a driver.</summary>
			static void deregisterDriver(const std::string& name);
			/// <summary>Get a database driver.</summary>
			static DatabaseDriverPtr getDriver(const std::string& name);
		private:
			DatabaseDriverManager();
			static DatabaseDriverManager& getInstance();
			std::map<std::string, DatabaseDriverPtr> _drivers;
		};
	}
}
