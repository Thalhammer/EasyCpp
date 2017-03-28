#pragma once
#include <string>
#include <memory>
#include <map>
#include <set>
#include <vector>
#include "Interface.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class DLL_EXPORT Manager
		{
		public:
			Manager();
			~Manager();

			void registerInterface(InterfacePtr iface);

			void loadPlugin(const std::string& name, const std::string& path, const std::vector<InterfacePtr>& server_ifaces = {});
			void loadPluginFromMemory(const std::string& name, const std::vector<uint8_t>& data, const std::vector<InterfacePtr>& server_ifaces = {});
			void deinitPlugin(const std::string& name);
			bool canUnloadPlugin(const std::string& name) const;
			void unloadPlugin(const std::string& name);
			std::set<std::string> getPlugins() const;

			void setAutoRegisterExtensions(bool v);
			bool isAutoRegisterExtensions() const;

			template <typename T>
			std::shared_ptr<T> getInterface(const std::string& pluginname) const
			{
				auto ptr = getInterface(pluginname, T::INTERFACE_NAME, T::INTERFACE_VERSION);
				auto n = std::dynamic_pointer_cast<T>(ptr);
				if (!n)
					throw std::runtime_error("Interface is not a instance of " + std::string(typeid(T).name()));
				return n;
			}
			template <typename T>
			bool hasInterface(const std::string& pluginname) const
			{
				return hasInterface(pluginname, T::INTERFACE_NAME, T::INTERFACE_VERSION);
			}
		private:
			class Plugin;

			InterfacePtr getInterface(const std::string& pluginname, const std::string& ifacename, uint64_t version) const;
			bool hasInterface(const std::string& pluginname, const std::string& ifacename, uint64_t version) const;

			typedef std::map<std::pair<std::string, uint64_t>, InterfacePtr> interface_map_t;
			interface_map_t _server_ifaces;
			std::map<std::string, std::shared_ptr<Plugin>> _plugins;
			bool _autoregister;
		};
	}
}