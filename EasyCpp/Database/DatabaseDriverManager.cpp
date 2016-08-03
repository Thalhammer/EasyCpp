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
			auto data = getInstance()._drivers.lock();
			std::vector<std::string> res;
			for (const auto& i : *data) res.push_back(i.first);
			return res;
		}

		void DatabaseDriverManager::registerDriver(const std::string & name, DatabaseDriverPtr driver)
		{
			auto data = getInstance()._drivers.lock();
			if (data->count(name) != 0)
				throw std::invalid_argument("A driver with this name is already registered");
			data->insert({ name, driver });
		}

		void DatabaseDriverManager::deregisterDriver(const std::string & name)
		{
			auto& instance = getInstance();
			instance._drivers->erase(name);
		}

		DatabaseDriverPtr DatabaseDriverManager::getDriver(const std::string & name)
		{
			auto data = getInstance()._drivers.lock();
			if (data->count(name) != 1)
				throw std::out_of_range("Driver not found");
			return data->at(name);
		}
	}
}
