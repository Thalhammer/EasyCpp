#include "Plugin.h"

namespace EasyCpp
{
	namespace Plugin
	{
		Manager::Plugin::Plugin(std::string name, std::string path, const interface_map_t & server_ifaces)
			:_name(name), _path(path), _lib(path), _unload_protect(std::make_shared<uint8_t>())
		{
			auto create_fn = _lib.getFunction<BaseInterface*()>("createBaseInterface");
			auto delete_fn = _lib.getFunction<void(BaseInterface*)>("deleteBaseInterface");
			_baseiface.reset(create_fn(), delete_fn);

			InitArgs args;
			args.setUnloadProtect(_unload_protect);
			for (auto e : server_ifaces)
				args.appendServerInterface(e.second);

			_baseiface->init(args);
			for (auto e : args.getPluginInterfaces())
				_interfaces.insert({ {e->getName(), e->getVersion()}, e });
		}

		Manager::Plugin::~Plugin()
		{
			_interfaces.clear();
			_baseiface.reset();
		}

		void Manager::Plugin::deinit()
		{
			// Clear references
			_interfaces.clear();
			_baseiface->deinit();
		}

		bool Manager::Plugin::canUnload() const
		{
			return _unload_protect.unique();
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
			if (!_interfaces.count({ ifacename, version }))
				throw std::runtime_error("Interface not found");
			return _interfaces.at({ ifacename,version });
		}

		bool Manager::Plugin::hasInterface(const std::string & ifacename, uint64_t version) const
		{
			return _interfaces.count({ ifacename,version }) != 0;
		}
	}
}