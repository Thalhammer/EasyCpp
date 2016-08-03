#include "VFSProviderManager.h"

namespace EasyCpp
{
	namespace VFS
	{
		std::vector<std::string> VFSProviderManager::getAvailableProviders()
		{
			auto data = getInstance()._providers.lock();
			std::vector<std::string> res;
			for (auto& i : *data) res.push_back(i.first);
			return res;
		}

		bool VFSProviderManager::hasProvider(const std::string & name)
		{
			return getInstance()._providers->count(name) != 0;
		}

		void VFSProviderManager::registerProvider(const std::string & name, VFSProviderFn driver)
		{
			getInstance()._providers->insert({ name, driver });
		}

		void VFSProviderManager::deregisterProvider(const std::string & name)
		{
			getInstance()._providers->erase(name);
		}

		VFSProviderPtr VFSProviderManager::getProvider(const std::string & name, const Bundle & options)
		{
			return getInstance()._providers->at(name)(options);
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
