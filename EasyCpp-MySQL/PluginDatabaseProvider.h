#pragma once
#include <Plugin/IPluginDatabaseProvider.h>

namespace EasyCppMySql
{
	class PluginDatabaseProvider : public EasyCpp::Plugin::IPluginDatabaseProvider
	{
	public:
		PluginDatabaseProvider(std::shared_ptr<void> unloadp);
		virtual ~PluginDatabaseProvider();
		// Geerbt über IPluginDatabaseProvider
		virtual std::map<std::string, EasyCpp::Database::DatabaseDriverPtr> getDriverMap() override;
	private:
		std::shared_ptr<void> _unloadp;
		std::map<std::string, EasyCpp::Database::DatabaseDriverPtr> _driver_map;
	};
}