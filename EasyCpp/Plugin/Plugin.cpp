#include "Plugin.h"
#include "IPluginCanUnload.h"


namespace EasyCpp
{
	namespace Plugin
	{
		Manager::Plugin::Plugin(std::string name, std::string path, interface_map_t & server_ifaces)
			:_name(name), _path(path), _lib(path)
		{
			auto create_fn = _lib.getFunction<BaseInterface*()>("createBaseInterface");
			auto delete_fn = _lib.getFunction<void(BaseInterface*)>("deleteBaseInterface");
			_baseiface.reset(create_fn(), delete_fn);
			
			InitArgs args;
			for (auto e : server_ifaces)
				for (auto e2 : e.second)
					args.appendServerInterface(e2.second);

			_baseiface->init(args);
			//_name = args.getPluginName();
			for (auto e : args.getPluginInterfaces())
			{
				_interfaces[e->getName()][e->getVersion()] = e;
			}
		}

		Manager::Plugin::~Plugin()
		{
			_interfaces.clear();
			_baseiface.reset();
		}

		std::string Manager::Plugin::getName() const
		{
			return _name;
		}

		std::string Manager::Plugin::getPath() const
		{
			return _path;
		}

		DynLib & Manager::Plugin::getDynLib()
		{
			return _lib;
		}

		InterfacePtr Manager::Plugin::getInterface(const std::string & ifacename, uint64_t version) const
		{
			if (!_interfaces.count(ifacename))
				throw std::runtime_error("Interface not found");
			if (!_interfaces.at(ifacename).count(version))
				throw std::runtime_error("Interface version not found");
			return _interfaces.at(ifacename).at(version);
		}

		bool Manager::Plugin::hasInterface(const std::string & ifacename, uint64_t version) const
		{
			return _interfaces.count(ifacename) && _interfaces.at(ifacename).count(version);
		}

		bool Manager::Plugin::canUnload() const
		{
			if (this->hasInterface<IPluginCanUnload>())
			{
				auto iface = this->getInterface<IPluginCanUnload>();
				return iface->canUnload();
			}
			else return true;
		}

		void Manager::Plugin::deinit()
		{
			// Clear references
			_interfaces.clear();

			DeinitArgs args;
			_baseiface->deinit(args);
			if (args.isCanceled())
				throw std::runtime_error("Failed to deinit plugin:" + args.getCancelString());
		}
	}
}