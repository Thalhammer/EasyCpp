#pragma once
#include <string>
#include <memory>

namespace EasyCpp
{
	namespace Plugin
	{
		class PluginInterfaceBase {
		public:
			virtual ~PluginInterfaceBase() {}
			virtual std::string getName() const = 0;
			virtual uint64_t getVersion() const = 0;
		};

		template<typename T>
		class PluginInterface : public PluginInterfaceBase
		{
		public:
			virtual ~PluginInterface() {}
			virtual std::string getName() const { return T::INTERFACE_NAME; }
			virtual uint64_t getVersion() const { return T::INTERFACE_VERSION; }
		};
		typedef std::shared_ptr<PluginInterfaceBase> PluginInterfacePtr;
	}
}