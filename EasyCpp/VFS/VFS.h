#pragma once
#include "../DllExport.h"
#include "Path.h"
#include "VFSProvider.h"
#include "../ThreadSafe.h"
#include <unordered_map>

namespace EasyCpp
{
	namespace VFS
	{
		class VFS;
		typedef std::shared_ptr<VFS> VFSPtr;
		class DLL_EXPORT VFS
		{
		public:
			static VFSPtr getDefaultVFS();

			VFS();
			VFS(const VFS&) = default;
			VFS(VFS&&) = default;
			virtual ~VFS();

			void addMountPoint(const Path& base, VFSProviderPtr provider);
			void removeMountPoint(const Path& base);
			std::unordered_map<std::string, VFSProviderPtr> getMountPoints() const;

			bool exists(const Path& path) const;
			void remove(const Path& path) const;
			void rename(const Path& p, const Path& target) const;
			std::vector<Path> getFiles(const Path& path) const;

			InputOutputStreamPtr openIO(const Path& path) const;
			InputStreamPtr openInput(const Path& path) const;
			OutputStreamPtr openOutput(const Path& path) const;
		private:
			std::string matchMountPoint(const Path& path) const;
			ThreadSafe<std::unordered_map<std::string, VFSProviderPtr>, std::recursive_mutex> _mountpoints;
		};
	}
}