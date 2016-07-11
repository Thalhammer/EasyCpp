#pragma once
#include "Path.h"
#include <vector>
#include <memory>
#include "InputOutputStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		class VFSProvider
		{
		public:
			virtual bool ready() = 0;
			virtual bool exists(const Path& p) = 0;
			virtual void remove(const Path& p) = 0;
			virtual void rename(const Path& p, const Path& target) = 0;
			virtual std::vector<Path> getFiles(const Path& p) = 0;

			virtual InputOutputStreamPtr openIO(const Path& path) = 0;
			virtual InputStreamPtr openInput(const Path& path) = 0;
			virtual OutputStreamPtr openOutput(const Path& path) = 0;
		};
		typedef std::shared_ptr<VFSProvider> VFSProviderPtr;
	}
}