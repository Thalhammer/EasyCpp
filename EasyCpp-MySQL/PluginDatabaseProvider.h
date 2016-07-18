#pragma once
#include <Plugin/IPluginDatabaseProvider.h>
#include "RefCounted.h"

namespace EasyCppMySql
{
	class PluginDatabaseProvider : public EasyCpp::Plugin::IPluginDatabaseProvider, RefCounted
	{
	public:
		PluginDatabaseProvider();
		virtual ~PluginDatabaseProvider();
		// Geerbt �ber IPluginDatabaseProvider
		virtual std::map<std::string, EasyCpp::Database::DatabaseDriverPtr> getDriverMap() override;
	private:
		std::map<std::string, EasyCpp::Database::DatabaseDriverPtr> _driver_map;
	};
}