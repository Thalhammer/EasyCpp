#pragma once
#include "Interface.h"
#include "../Database/DatabaseDriver.h"
#include <map>
#include <string>

namespace EasyCpp
{
	namespace Plugin
	{
		class IPluginDatabaseProvider : public Interface<IPluginDatabaseProvider>
		{
		public:
			static constexpr const char* INTERFACE_NAME = "IPluginDatabaseProvider";
			static constexpr uint64_t INTERFACE_VERSION = 0;

			virtual ~IPluginDatabaseProvider() {}
			virtual std::map<std::string, EasyCpp::Database::DatabaseDriverPtr> getDriverMap() = 0;
		};
	}
}