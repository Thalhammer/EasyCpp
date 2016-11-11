#pragma once
#include "Hash.h"
#include "../ThreadSafe.h"
#include <vector>
#include <map>
#include <functional>

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT HashManager
		{
		public:
			typedef std::function<HashPtr()> HashProviderFn;
			/// <summary>Get a list of registered hashing engines.</summary>
			static std::vector<std::string> getAvailableHashes();
			/// <summary>Get a hashing engine instance.</summary>
			static HashPtr getHash(const std::string& hash);
			/// <summary>Register a hashing engine.</summary>
			static void registerHash(const std::string& hash, HashProviderFn createfn);
			/// <summary>Deregister hashing engine.</summary>
			static void deregisterHash(const std::string& hash);
		private:
			HashManager();
			static HashManager& getInstance();

			ThreadSafe<std::map<std::string, HashProviderFn>> _providers;
		};
	}
}