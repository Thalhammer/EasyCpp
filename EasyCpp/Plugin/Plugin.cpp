#include "Plugin.h"
#include "IPluginCanUnload.h"


namespace EasyCpp
{
	namespace Plugin
	{
		PluginManager::Plugin::Plugin(std::string name, std::string path, interface_map_t & server_ifaces)
			:_name(name), _path(path), _lib(path)
		{
			auto create_fn = _lib.getFunction<PluginBaseInterface*()>("createBaseInterface");
			auto delete_fn = _lib.getFunction<void(PluginBaseInterface*)>("deleteBaseInterface");
			_baseiface.reset(create_fn(), delete_fn);
			std::vector<PluginInterfacePtr> sifaces;
			for (auto e : server_ifaces)
				for (auto e2 : e.second)
					sifaces.push_back(e2.second);

			auto ifaces = _baseiface->init(sifaces);
			for (auto e : ifaces)
			{
				_interfaces[e->getName()][e->getVersion()] = e;
			}
		}

		PluginManager::Plugin::~Plugin()
		{
			_interfaces.clear();
			_baseiface.reset();
		}

		std::string PluginManager::Plugin::getName() const
		{
			return _name;
		}

		std::string PluginManager::Plugin::getPath() const
		{
			return _path;
		}

		DynLib & PluginManager::Plugin::getDynLib()
		{
			return _lib;
		}

		PluginInterfacePtr PluginManager::Plugin::getInterface(const std::string & ifacename, uint64_t version) const
		{
			if (!_interfaces.count(ifacename))
				throw std::runtime_error("Interface not found");
			if (!_interfaces.at(ifacename).count(version))
				throw std::runtime_error("Interface version not found");
			return _interfaces.at(ifacename).at(version);
		}

		bool PluginManager::Plugin::hasInterface(const std::string & ifacename, uint64_t version) const
		{
			return _interfaces.count(ifacename) && _interfaces.at(ifacename).count(version);
		}

		bool PluginManager::Plugin::canUnload() const
		{
			if (this->hasInterface<IPluginCanUnload>())
			{
				auto iface = this->getInterface<IPluginCanUnload>();
				return iface->canUnload();
			}
			else return true;
		}
	}
}