#include "InitArgs.h"
namespace EasyCpp
{
	namespace Plugin
	{
		std::vector<InterfacePtr> InitArgs::getServerInterfaces()
		{
			return _server_interfaces;
		}

		std::vector<InterfacePtr> InitArgs::getPluginInterfaces()
		{
			return _plugin_interfaces;
		}

		void InitArgs::setServerInterfaces(std::vector<InterfacePtr> ifaces)
		{
			_server_interfaces = ifaces;
		}

		void InitArgs::setPluginInterfaces(std::vector<InterfacePtr> ifaces)
		{
			_plugin_interfaces = ifaces;
		}

		void InitArgs::appendServerInterface(InterfacePtr iface)
		{
			_server_interfaces.push_back(iface);
		}

		void InitArgs::appendPluginInterface(InterfacePtr iface)
		{
			_plugin_interfaces.push_back(iface);
		}

		std::string InitArgs::getPluginName()
		{
			return _plugin_name;
		}

		void InitArgs::setPluginName(const std::string& name)
		{
			_plugin_name = name;
		}
	}
}