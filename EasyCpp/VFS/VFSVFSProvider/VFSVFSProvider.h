#pragma once
#include "../VFSProvider.h"
#include "../VFS.h"

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT VFSVFSProvider : public VFSProvider
		{
		public:
			VFSVFSProvider(VFSPtr ptr, Path base = Path("/"));
			virtual ~VFSVFSProvider();
			// Geerbt über VFSProvider
			virtual bool ready() override;
			virtual bool exists(const Path & p) override;
			virtual void remove(const Path & p) override;
			virtual void rename(const Path& p, const Path& target) override;
			virtual std::vector<Path> getFiles(const Path & p) override;
			virtual InputOutputStreamPtr openIO(const Path & path) override;
			virtual InputStreamPtr openInput(const Path & path) override;
			virtual OutputStreamPtr openOutput(const Path & path) override;
		private:
			Path _base;
			VFSPtr _vfs;
		};
	}
}