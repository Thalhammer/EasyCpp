#include "HashManager.h"

namespace EasyCpp
{
	namespace Hash
	{
		std::vector<std::string> HashManager::getAvailableHashes()
		{
			auto& instance = getInstance();
			std::vector<std::string> res;
			for (auto&& i : instance._providers) res.push_back(i.first);
			return res;
		}

		HashPtr HashManager::getHash(const std::string & hash)
		{
			auto& instance = getInstance();
			if (!(instance._providers.count(hash) == 1))
				throw std::out_of_range("Hash not found");
			return instance._providers.at(hash)();
		}

		void HashManager::registerHash(const std::string & hash, HashProviderFn createfn)
		{
			auto& instance = getInstance();
			if (!(instance._providers.count(hash) == 0))
				throw std::runtime_error("Hash already registered");
			instance._providers.insert({ hash, createfn });
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
