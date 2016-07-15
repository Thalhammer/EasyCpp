#pragma once
#include "Hash.h"
#include <vector>
#include <map>
#include <functional>

namespace EasyCpp
{
	namespace Hash
	{
		class HashManager
		{
		public:
			typedef std::function<HashPtr()> HashProviderFn;
			static std::vector<std::string> getAvailableHashes();
			static HashPtr getHash(const std::string& hash);
			static void registerHash(const std::string& hash, HashProviderFn createfn);
		private:
			HashManager();
			static HashManager& getInstance();

			std::map<std::string, HashProviderFn> _providers;
		};
	}
}