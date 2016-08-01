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
			/// <summary>Get a list of registered hashing engines.</summary>
			static std::vector<std::string> getAvailableHashes();
			/// <summary>Get a hashing engine instance.</summary>
			static HashPtr getHash(const std::string& hash);
			/// <summary>Register a hashing engine.</summary>
			static void registerHash(const std::string& hash, HashProviderFn createfn);
		private:
			HashManager();
			static HashManager& getInstance();

			std::map<std::string, HashProviderFn> _providers;
		};
	}
}