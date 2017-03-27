#include "PluginInterface.h"
#include "PluginDatabaseProvider.h"

#include <my_global.h>
#include <mysql.h>

using EasyCpp::Plugin::InitArgs;

namespace EasyCppMySql
{
	void PluginInterface::deinit()
	{
		mysql_library_end();
	}

	void PluginInterface::pluginInit(InitArgs & args)
	{
		args.setPluginName("EasyCpp-Mysql");
		args.appendPluginInterface(std::make_shared<PluginDatabaseProvider>(args.getUnloadProtect()));

		mysql_library_init(0, nullptr, nullptr);
	}
}

EASYCPP_PLUGIN_ENTRY(EasyCppMySql::PluginInterface)