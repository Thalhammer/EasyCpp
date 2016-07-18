#include "PluginInterface.h"
#include "PluginDatabaseProvider.h"
#include "RefCounted.h"

using EasyCpp::Plugin::DeinitArgs;
using EasyCpp::Plugin::InitArgs;

namespace EasyCppMySql
{
	void PluginInterface::deinit(DeinitArgs & args)
	{
		if (!RefCounted::canDeInit())
			args.cancel(EasyCpp::Plugin::DeinitArgs::Reason::IN_USE, "Plugin still in use");
	}

	void PluginInterface::pluginInit(InitArgs & args)
	{
		args.setPluginName("EasyCpp-Mysql");
		args.appendPluginInterface(std::make_shared<PluginDatabaseProvider>());
	}
}

EASYCPP_PLUGIN_ENTRY(EasyCppMySql::PluginInterface)