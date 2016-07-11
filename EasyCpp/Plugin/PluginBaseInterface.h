#pragma once
#include <vector>
#include "PluginInterface.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class DLL_EXPORT PluginBaseInterface
		{
		public:
			virtual ~PluginBaseInterface() {}
			virtual std::vector<PluginInterfacePtr> init(std::vector<PluginInterfacePtr> server_interfaces) = 0;
			virtual void deinit() = 0;
		};
	}
}