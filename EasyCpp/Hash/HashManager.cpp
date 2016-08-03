#include "HashManager.h"

namespace EasyCpp
{
	namespace Hash
	{
		std::vector<std::string> HashManager::getAvailableHashes()
		{
			auto data = getInstance()._providers.lock();
			std::vector<std::string> res;
			for (auto& i : *data) res.push_back(i.first);
			return res;
		}

		HashPtr HashManager::getHash(const std::string & hash)
		{
			auto data = getInstance()._providers.lock();
			if (!(data->count(hash) == 1))
				throw std::out_of_range("Hash not found");
			return data->at(hash)();
		}

		void HashManager::registerHash(const std::string & hash, HashProviderFn createfn)
		{
			auto data = getInstance()._providers.lock();
			if (!(data->count(hash) == 0))
				throw std::runtime_error("Hash already registered");
			data->insert({ hash, createfn });
		}

		void HashManager::deregisterHash(const std::string & hash)
		{
			auto data = getInstance()._providers.lock();
			data->erase(hash);
		}

		HashManager::HashManager()
		{
		}

		HashManager & HashManager::getInstance()
		{
			static HashManager mgr;
			return mgr;
		}
	}
}
