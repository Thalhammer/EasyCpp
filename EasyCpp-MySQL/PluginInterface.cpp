#include "PluginInterface.h"
#include "PluginDatabaseProvider.h"
#include "RefCounted.h"

#include <my_global.h>
#include <mysql.h>

using EasyCpp::Plugin::DeinitArgs;
using EasyCpp::Plugin::InitArgs;

namespace EasyCppMySql
{
	void PluginInterface::deinit(DeinitArgs & args)
	{
		if (!RefCounted::canDeInit())
			args.cancel(EasyCpp::Plugin::DeinitArgs::Reason::IN_USE, "Plugin still in use");

		mysql_library_end();
	}

	void PluginInterface::pluginInit(InitArgs & args)
	{
		args.setPluginName("EasyCpp-Mysql");
		args.appendPluginInterface(std::make_shared<PluginDatabaseProvider>());

		mysql_library_init(0, nullptr, nullptr);
	}
}

EASYCPP_PLUGIN_ENTRY(EasyCppMySql::PluginInterface)