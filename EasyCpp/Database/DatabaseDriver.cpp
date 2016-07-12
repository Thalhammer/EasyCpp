#include "DatabaseDriver.h"

namespace EasyCpp
{
	namespace Database
	{
		std::map<std::string, DatabaseDriverPtr> DatabaseDriver::_drivers;
		std::vector<std::string> DatabaseDriver::getAvailableDrivers()
		{
			std::vector<std::string> res;
			for (const auto& i : _drivers) res.push_back(i.first);
			return res;
		}

		void DatabaseDriver::registerDriver(const std::string & name, DatabaseDriverPtr driver)
		{
			if (_drivers.count(name) != 0)
				throw std::invalid_argument("A driver with this name is already registered");
			_drivers.insert({ name, driver });
		}

		DatabaseDriverPtr DatabaseDriver::getDriver(const std::string & name)
		{
			if (_drivers.count(name) != 1)
				throw std::out_of_range("Driver not found");
			return _drivers.at(name);
		}
	}
}