#pragma once
#include <map>
#include "PluginBaseInterface.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class DLL_EXPORT PluginBase : public PluginBaseInterface
		{
		public:
			virtual ~PluginBase();
			virtual std::vector<PluginInterfacePtr> init(std::vector<PluginInterfacePtr> server_interfaces) override;
			template <typename T>
			std::shared_ptr<T> getServerInterface()
			{
				auto ptr = getServerInterface(T::INTERFACE_NAME, T::INTERFACE_VERSION);
				auto n = std::dynamic_pointer_cast<T>(ptr);
				if (!n)
					throw std::runtime_error("Interface is not a instance of " + std::string(typeid(T).name()));
				return n;
			}
			template <typename T>
			bool hasServerInterface()
			{
				return hasServerInterface(T::INTERFACE_NAME, T::INTERFACE_VERSION);
			}
		protected:
			virtual std::vector<PluginInterfacePtr> init() = 0;
		private:
			PluginInterfacePtr getServerInterface(const std::string& ifacename, uint64_t version);
			bool hasServerInterface(const std::string& ifacename, uint64_t version);
			std::map<std::string, std::map<uint64_t, PluginInterfacePtr>> _server_interfaces;
		};
	}
}