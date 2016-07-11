#include "PluginManager.h"
#include "Plugin.h"
#include <fstream>

namespace EasyCpp
{
	namespace Plugin
	{
		PluginManager::PluginManager()
		{
		}

		PluginManager::~PluginManager()
		{
		}

		void PluginManager::registerInterface(PluginInterfacePtr iface)
		{
			auto name = iface->getName();
			auto rev = iface->getVersion();
			if (_server_ifaces.count(name) && _server_ifaces[name].count(rev))
			{
				throw std::logic_error("Interface already registered");
			}
			_server_ifaces[name][rev] = iface;
		}

		void PluginManager::loadPlugin(const std::string & name, const std::string & path, const std::vector<PluginInterfacePtr>& server_ifaces)
		{
			if (_plugins.count(name))
				throw std::runtime_error("Plugin name already used");
			interface_map_t tmap = _server_ifaces;
			for (auto& e : server_ifaces)
				tmap[e->getName()][e->getVersion()] = e;
			auto plugin = std::make_shared<Plugin>(name, path, tmap);
			_plugins[name] = plugin;
		}

		void PluginManager::loadPluginFromMemory(const std::string & name, const std::vector<uint8_t>& data, const std::vector<PluginInterfacePtr>& server_ifaces)
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

		PluginInterfacePtr PluginManager::getInterface(const std::string & pluginname, const std::string & ifacename, uint64_t version)
		{
			if (_plugins.count(pluginname) == 0)
				throw std::runtime_error("Plugin not found");
			return _plugins[pluginname]->getInterface(ifacename, version);
		}

		bool PluginManager::hasInterface(const std::string & pluginname, const std::string & ifacename, uint64_t version)
		{
			if (_plugins.count(pluginname) == 0)
				throw std::runtime_error("Plugin not found");
			return _plugins[pluginname]->hasInterface(ifacename, version);
		}

		void PluginManager::unloadPlugin(const std::string & name)
		{
			if (!_plugins.count(name))
				throw std::runtime_error("Plugin not found");
			if (!_plugins[name]->canUnload())
				throw std::runtime_error("Plugin is in use");
			_plugins.erase(name);
		}

		std::vector<std::string> PluginManager::getPlugins()
		{
			std::vector<std::string> res;
			for (auto e : _plugins)
				res.push_back(e.first);
			return res;
		}
	}
}