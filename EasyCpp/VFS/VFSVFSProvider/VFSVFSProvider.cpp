#include "VFSVFSProvider.h"
#include "../../AutoInit.h"
#include "../VFSProviderManager.h"

namespace EasyCpp
{
	namespace VFS
	{
		AUTO_INIT({
			VFSProviderManager::registerProvider("vfs", [](const Bundle& options) {
				auto vfs = std::make_shared<VFS>();
				if (options.isSet("mountpoints")) {
					for (auto& i : options.get<Bundle>("mountpoints"))
					{
						auto type = i.second.as<Bundle>().get<std::string>("type");
						auto mnt_options = i.second.as<Bundle>().get<Bundle>("options");
						vfs->addMountPoint(i.first, VFSProviderManager::getProvider(type, mnt_options));
					}
				}
				return std::make_shared<VFSVFSProvider>(vfs, options.get<std::string>("base"));
			});
		})

		VFSVFSProvider::VFSVFSProvider(VFSPtr ptr, Path base)
			: _base(base), _vfs(ptr)
		{
		}

		VFSVFSProvider::~VFSVFSProvider()
		{
		}

		bool VFSVFSProvider::ready()
		{
			return !(!_vfs);
		}

		bool VFSVFSProvider::exists(const Path & p)
		{
			return _vfs->exists(_base + p);
		}

		void VFSVFSProvider::remove(const Path & p)
		{
			_vfs->remove(_base + p);
		}

		void VFSVFSProvider::rename(const Path & p, const Path & target)
		{
			_vfs->rename(_base + p, _base + target);
		}

		std::vector<Path> VFSVFSProvider::getFiles(const Path & p)
		{
			return _vfs->getFiles(_base + p);
		}

		InputOutputStreamPtr VFSVFSProvider::openIO(const Path & path)
		{
			return _vfs->openIO(_base + path);
		}

		InputStreamPtr VFSVFSProvider::openInput(const Path & path)
		{
			return _vfs->openInput(_base + path);
		}

		OutputStreamPtr VFSVFSProvider::openOutput(const Path & path)
		{
			return _vfs->openOutput(_base + path);
		}

	}
}
