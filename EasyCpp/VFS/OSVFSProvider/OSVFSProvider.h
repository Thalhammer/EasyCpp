#pragma once
#include "../VFSProvider.h"
#include "../../DllExport.h"

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT OSVFSProvider : public VFSProvider
		{
		public:
			OSVFSProvider(const std::string& base);
			virtual ~OSVFSProvider();

			// Geerbt über VFSProvider
			virtual bool ready() override;
			virtual bool exists(const Path & p) override;
			virtual void remove(const Path & p) override;
			virtual void rename(const Path & p, const Path & target) override;
			virtual std::vector<Path> getFiles(const Path & p) override;
			virtual InputOutputStreamPtr openIO(const Path& path) override;
			virtual InputStreamPtr openInput(const Path& path) override;
			virtual OutputStreamPtr openOutput(const Path& path) override;

			static std::string getCurrentWorkingDirectory();
		private:
			std::string _base;
		};
	}
}