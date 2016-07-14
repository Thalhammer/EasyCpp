#include "DatabaseDriverManager.h"

namespace EasyCpp
{
	namespace Database
	{
		DatabaseDriverManager& DatabaseDriverManager::getInstance()
		{
			static DatabaseDriverManager manager;
			return manager;
		}

		DatabaseDriverManager::DatabaseDriverManager()
		{

		}

		std::vector<std::string> DatabaseDriverManager::getAvailableDrivers()
		{
			auto& instance = getInstance();
			std::vector<std::string> res;
			for (const auto& i : instance._drivers) res.push_back(i.first);
			return res;
		}

		void DatabaseDriverManager::registerDriver(const std::string & name, DatabaseDriverPtr driver)
		{
			auto& instance = getInstance();
			if (instance._drivers.count(name) != 0)
				throw std::invalid_argument("A driver with this name is already registered");
			instance._drivers.insert({ name, driver });
		}

		DatabaseDriverPtr DatabaseDriverManager::getDriver(const std::string & name)
		{
			auto& instance = getInstance();
			if (instance._drivers.count(name) != 1)
				throw std::out_of_range("Driver not found");
			return instance._drivers.at(name);
		}
	}
}
