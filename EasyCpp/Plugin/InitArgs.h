#pragma once
#include <string>
#include <vector>
#include "Interface.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class DLL_EXPORT InitArgs
		{
		public:
			std::vector<InterfacePtr> getServerInterfaces();
			std::vector<InterfacePtr> getPluginInterfaces();
			void setServerInterfaces(std::vector<InterfacePtr> ifaces);
			void setPluginInterfaces(std::vector<InterfacePtr> ifaces);

			void appendServerInterface(InterfacePtr iface);
			void appendPluginInterface(InterfacePtr iface);

			std::string getPluginName();
			void setPluginName(const std::string& name);
		private:
			std::vector<InterfacePtr> _server_interfaces;
			std::vector<InterfacePtr> _plugin_interfaces;

			std::string _plugin_name;
		};
	}
}