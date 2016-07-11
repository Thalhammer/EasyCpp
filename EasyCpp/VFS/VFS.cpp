#include "VFS.h"
#include "../StringAlgorithm.h"
#include "OSVFSProvider/OSVFSProvider.h"

namespace EasyCpp
{
	namespace VFS
	{
		VFSPtr VFS::getDefaultVFS()
		{
			static VFSPtr defaultVFS;
			if (defaultVFS == nullptr)
			{
				defaultVFS = std::make_shared<VFS>();
				defaultVFS->addMountPoint(Path("/"), std::make_shared<OSVFSProvider>(OSVFSProvider::getCurrentWorkingDirectory()));
			}
			return defaultVFS;
		}

		VFS::VFS()
		{
		}

		VFS::~VFS()
		{
		}

		void VFS::addMountPoint(const Path & base, VFSProviderPtr provider)
		{
			_mountpoints[base.getDirName()] = provider;
		}

		void VFS::removeMountPoint(const Path & base)
		{
			if (_mountpoints.count(base.getDirName()) != 1)
				throw std::runtime_error("Mountpoint not found");
			_mountpoints.erase(base.getDirName());
		}

		std::unordered_map<std::string, VFSProviderPtr> VFS::getMountPoints() const
		{
			return _mountpoints;
		}

		bool VFS::exists(const Path & path) const
		{
			std::string mnt = matchMountPoint(path);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			Path relpath(path.getString().substr(mnt.size() - 1));
			VFSProviderPtr provider = _mountpoints.at(mnt);
			return provider->exists(relpath);
		}

		void VFS::remove(const Path & path) const
		{
			std::string mnt = matchMountPoint(path);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			Path relpath(path.getString().substr(mnt.size() - 1));
			VFSProviderPtr provider = _mountpoints.at(mnt);
			provider->remove(relpath);
		}

		void VFS::rename(const Path & p, const Path & target) const
		{
			std::string mnt = matchMountPoint(p);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			std::string mnt2 = matchMountPoint(target);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			Path relpath1(p.getString().substr(mnt.size() - 1));
			Path relpath2(target.getString().substr(mnt2.size() - 1));
			if (mnt == mnt2)
			{
				VFSProviderPtr provider = _mountpoints.at(mnt);
				provider->rename(relpath1, relpath2);
			}
			else {
				VFSProviderPtr provider1 = _mountpoints.at(mnt);
				VFSProviderPtr provider2 = _mountpoints.at(mnt);
				auto is = provider1->openInput(relpath1);
				auto os = provider2->openOutput(relpath2);
				while (is->isGood() && os->isGood())
				{
					auto data = is->read(1024);
					os->write(data);
				}
			}
		}

		std::vector<Path> VFS::getFiles(const Path & path) const
		{
			std::string mnt = matchMountPoint(path);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			Path relpath(path.getString().substr(mnt.size() - 1));
			VFSProviderPtr provider = _mountpoints.at(mnt);
			auto files = provider->getFiles(relpath);
			std::vector<Path> res;
			for (const auto& e : files)
			{
				res.push_back(Path(mnt + e.getString().substr(1)));
			}
			return res;
		}

		InputOutputStreamPtr VFS::openIO(const Path & path) const
		{
			std::string mnt = matchMountPoint(path);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			Path relpath(path.getString().substr(mnt.size() - 1));
			VFSProviderPtr provider = _mountpoints.at(mnt);
			return provider->openIO(relpath);
		}

		InputStreamPtr VFS::openInput(const Path & path) const
		{
			std::string mnt = matchMountPoint(path);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			Path relpath(path.getString().substr(mnt.size() - 1));
			VFSProviderPtr provider = _mountpoints.at(mnt);
			return provider->openInput(relpath);
		}

		OutputStreamPtr VFS::openOutput(const Path & path) const
		{
			std::string mnt = matchMountPoint(path);
			if (mnt == "")
				throw std::runtime_error("Failed to find mountpoint");
			Path relpath(path.getString().substr(mnt.size() - 1));
			VFSProviderPtr provider = _mountpoints.at(mnt);
			return provider->openOutput(relpath);
		}

		std::string VFS::matchMountPoint(const Path & path) const
		{
			auto parts = stringSplit("/", path.getDirName());
			std::string match = "";
			for (auto& e : _mountpoints)
			{
				auto mnt_parts = stringSplit("/", e.first);
				mnt_parts.erase(mnt_parts.begin() + mnt_parts.size() - 1);
				if (mnt_parts.size() > parts.size())
					continue;
				bool matched = true;
				for (size_t i = 0; i < mnt_parts.size(); i++)
				{
					if (mnt_parts[i] != parts[i])
					{
						matched = false;
						break;
					}
				}
				if (matched) {
					if (match.size() < e.first.size())
					{
						match = e.first;
					}
				}
			}
			return match;
		}
	}
}
