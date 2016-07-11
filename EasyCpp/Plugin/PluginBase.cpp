#include "PluginBase.h"

namespace EasyCpp
{
	namespace Plugin
	{
		PluginBase::~PluginBase()
		{
		}

		std::vector<PluginInterfacePtr> PluginBase::init(std::vector<PluginInterfacePtr> server_interfaces)
		{
			for (auto e : server_interfaces)
				_server_interfaces[e->getName()][e->getVersion()] = e;

			return init();
		}

		PluginInterfacePtr PluginBase::getServerInterface(const std::string & ifacename, uint64_t version)
		{
			if (!_server_interfaces.count(ifacename))
				throw std::runtime_error("Interface not found");
			if (!_server_interfaces.at(ifacename).count(version))
				throw std::runtime_error("Interface version not found");
			return _server_interfaces.at(ifacename).at(version);
		}

		bool PluginBase::hasServerInterface(const std::string & ifacename, uint64_t version)
		{
			return _server_interfaces.count(ifacename) && _server_interfaces.at(ifacename).count(version);
		}
	}
}