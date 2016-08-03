#pragma once
#include "VFSProvider.h"
#include "../Bundle.h"
#include "../ThreadSafe.h"
#include <functional>

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT VFSProviderManager
		{
		public:
			typedef std::function<VFSProviderPtr(const Bundle& options)> VFSProviderFn;
			static std::vector<std::string> getAvailableProviders();
			static bool hasProvider(const std::string& name);
			static void registerProvider(const std::string& name, VFSProviderFn driver);
			static void deregisterProvider(const std::string& name);
			static VFSProviderPtr getProvider(const std::string& name, const Bundle& options);
		private:
			VFSProviderManager();
			static VFSProviderManager& getInstance();
			ThreadSafe<std::map<std::string, VFSProviderFn>> _providers;
		};
	}
}