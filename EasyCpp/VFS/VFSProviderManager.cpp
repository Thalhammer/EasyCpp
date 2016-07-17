#include "VFSProviderManager.h"

namespace EasyCpp
{
	namespace VFS
	{
		std::vector<std::string> VFSProviderManager::getAvailableProviders()
		{
			auto& instance = getInstance();
			std::vector<std::string> res;
			for (auto& i : instance._providers) res.push_back(i.first);
			return res;
		}

		bool VFSProviderManager::hasProvider(const std::string & name)
		{
			auto& instance = getInstance();
			return instance._providers.count(name) != 0;
		}

		void VFSProviderManager::registerProvider(const std::string & name, VFSProviderFn driver)
		{
			auto& instance = getInstance();
			instance._providers.insert({ name, driver });
		}

		void VFSProviderManager::deregisterProvider(const std::string & name)
		{
			auto& instance = getInstance();
			instance._providers.erase(name);
		}

		VFSProviderPtr VFSProviderManager::getProvider(const std::string & name, const Bundle & options)
		{
			auto& instance = getInstance();
			return instance._providers.at(name)(options);
		}

		VFSProviderManager::VFSProviderManager()
		{
		}

		VFSProviderManager & VFSProviderManager::getInstance()
		{
			static VFSProviderManager manager;
			return manager;
		}
	}
}
