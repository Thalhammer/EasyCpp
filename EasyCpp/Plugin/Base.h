#pragma once
#include <map>
#include "BaseInterface.h"

namespace EasyCpp
{
	namespace Plugin
	{
		class DLL_EXPORT Base : public BaseInterface
		{
		public:
			virtual ~Base();
			virtual void init(InitArgs& args) override;
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
			virtual void pluginInit(InitArgs& args) = 0;
		private:
			InterfacePtr getServerInterface(const std::string& ifacename, uint64_t version);
			bool hasServerInterface(const std::string& ifacename, uint64_t version);
			std::map<std::string, std::map<uint64_t,InterfacePtr>> _server_interfaces;
		};
	}
}