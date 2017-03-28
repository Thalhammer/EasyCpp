#include "Manager.h"
#include "Plugin.h"
#include <fstream>

#include "IPluginDatabaseProvider.h"
#include "IPluginScriptEngineFactoryProvider.h"
#include "../Database/DatabaseDriverManager.h"
#include "../Scripting/ScriptEngineManager.h"

namespace EasyCpp
{
	namespace Plugin
	{
		Manager::Manager()
		{
		}

		Manager::~Manager()
		{
		}

		void Manager::registerInterface(InterfacePtr iface)
		{
			auto name = iface->getName();
			auto rev = iface->getVersion();
			if (_server_ifaces.count({ name, rev }))
			{
				throw std::logic_error("Interface already registered");
			}
			_server_ifaces.insert({ { name, rev }, iface });
		}

		void Manager::loadPlugin(const std::string & name, const std::string & path, const std::vector<InterfacePtr>& server_ifaces)
		{
			if (_plugins.count(name))
				throw std::runtime_error("Plugin name already used");
			interface_map_t tmap = _server_ifaces;
			for (auto& e : server_ifaces)
				tmap.insert({ {e->getName(), e->getVersion()}, e });
			auto plugin = std::make_shared<Plugin>(name, path, tmap);
			_plugins.insert({ name,plugin });

			if (_autoregister) {
				// Autoregister EasyCpp Extensions
				if (this->hasInterface<IPluginDatabaseProvider>(name))
				{
					auto iface = this->getInterface<IPluginDatabaseProvider>(name);
					for (auto& e : iface->getDriverMap()) {
						Database::DatabaseDriverManager::registerDriver(e.first, e.second);
					}
				}
				if (this->hasInterface<IPluginScriptEngineFactoryProvider>(name))
				{
					auto iface = this->getInterface<IPluginScriptEngineFactoryProvider>(name);
					for (auto& e : iface->getFactories()) {
						Scripting::ScriptEngineManager::registerEngineFactory(e);
					}
				}
			}
		}

		void Manager::loadPluginFromMemory(const std::string & name, const std::vector<uint8_t>& data, const std::vector<InterfacePtr>& server_ifaces)
		{
			// Write data to a file and load it.
			char fname[L_tmpnam];
			tmpnam((char*)&fname);
			{
				std::ofstream stream;
				stream.open(fname, std::ofstream::out | std::ofstream::binary);
				stream.write((const char*)data.data(), data.size());
				stream.close();
			}
			this->loadPlugin(name, fname, server_ifaces);
		}

		void Manager::deinitPlugin(const std::string & name)
		{
			if (!_plugins.count(name))
				throw std::runtime_error("Plugin not found");
			auto plugin = _plugins.at(name);
			if (_autoregister) {
				// Autoregister EasyCpp Extensions
				if (this->hasInterface<IPluginDatabaseProvider>(name))
				{
					auto iface = this->getInterface<IPluginDatabaseProvider>(name);
					for (auto& e : iface->getDriverMap()) {
						Database::DatabaseDriverManager::deregisterDriver(e.first);
					}
				}
				if (this->hasInterface<IPluginScriptEngineFactoryProvider>(name))
				{
					auto iface = this->getInterface<IPluginScriptEngineFactoryProvider>(name);
					for (auto& e : iface->getFactories()) {
						Scripting::ScriptEngineManager::deregisterEngineFactory(e);
					}
				}
			}
			plugin->deinit();
		}

		bool Manager::canUnloadPlugin(const std::string & name) const
		{
			if (_plugins.count(name) == 0)
				throw std::runtime_error("Plugin not found");
			return _plugins.at(name)->canUnload();
		}

		void Manager::unloadPlugin(const std::string & name)
		{
			if (!_plugins.count(name))
				throw std::runtime_error("Plugin not found");
			auto plugin = _plugins.at(name);
			if (!plugin->canUnload())
				throw std::runtime_error("Plugin is not ready for unloading");
			_plugins.erase(name);
		}

		std::set<std::string> Manager::getPlugins() const
		{
			std::set<std::string> res;
			for (auto e : _plugins)
				res.insert(e.first);
			return res;
		}

		void Manager::setAutoRegisterExtensions(bool v)
		{
			_autoregister = v;
		}

		bool Manager::isAutoRegisterExtensions() const
		{
			return _autoregister;
		}

		InterfacePtr Manager::getInterface(const std::string & pluginname, const std::string & ifacename, uint64_t version) const
		{
			if (_plugins.count(pluginname) == 0)
				throw std::runtime_error("Plugin not found");
			return _plugins.at(pluginname)->getInterface(ifacename, version);
		}

		bool Manager::hasInterface(const std::string & pluginname, const std::string & ifacename, uint64_t version) const
		{
			if (_plugins.count(pluginname) == 0)
				throw std::runtime_error("Plugin not found");
			return _plugins.at(pluginname)->hasInterface(ifacename, version);
		}
	}
}