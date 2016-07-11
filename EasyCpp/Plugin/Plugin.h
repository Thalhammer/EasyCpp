#pragma once
#include <string>
#include "../DynLib.h"
#include "PluginBaseInterface.h"
#include "PluginManager.h"


namespace EasyCpp
{
	namespace Plugin
	{
		class PluginManager::Plugin
		{
		public:
			Plugin(std::string name, std::string path, interface_map_t& server_ifaces);
			~Plugin();
			std::string getName() const;
			std::string getPath() const;
			DynLib& getDynLib();
			PluginInterfacePtr getInterface(const std::string& ifacename, uint64_t version) const;
			bool hasInterface(const std::string& ifacename, uint64_t version) const;
			bool canUnload() const;

			template <typename T>
			std::shared_ptr<T> getInterface() const
			{
				auto ptr = getInterface(T::INTERFACE_NAME, T::INTERFACE_VERSION);
				auto n = std::dynamic_pointer_cast<T>(ptr);
				if (!n)
					throw std::runtime_error("Interface is not a instance of " + std::string(typeid(T).name()));
				return n;
			}
			template <typename T>
			bool hasInterface() const
			{
				return hasInterface(T::INTERFACE_NAME, T::INTERFACE_VERSION);
			}
		private:
			std::string _name;
			std::string _path;
			DynLib _lib;
			interface_map_t _interfaces;
			std::shared_ptr<PluginBaseInterface> _baseiface;
		};
	}
}