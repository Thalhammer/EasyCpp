#include "OSVFSProvider.h"
#include "OSVFSProvider.h"
#include "OSVFSInputStream.h"
#include "OSVFSOutputStream.h"
#include "OSVFSInputOutputStream.h"
#include "../StringAlgorithm.h"
#include "../../AutoInit.h"
#include "../VFSProviderManager.h"
#include <cstdio>

#if defined(__linux__)
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#else
#include <Windows.h>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")
#endif
namespace EasyCpp
{
	namespace VFS
	{
		AUTO_INIT({
			VFSProviderManager::registerProvider("os", [](const Bundle& options) {
				return std::make_shared<OSVFSProvider>(options.get<std::string>("base"));
			});
		})

		OSVFSProvider::OSVFSProvider(const std::string & base)
			:_base(base)
		{
		}

		OSVFSProvider::~OSVFSProvider()
		{
		}

		bool OSVFSProvider::ready()
		{
			return exists(_base);
		}

		bool OSVFSProvider::exists(const Path & p)
		{
#if defined(__linux__)
			std::string name = _base + p.getString();
			struct stat st;
			return stat(name.c_str(), &st) == 0;
#else
			std::string name = _base + stringReplace(p.getString(), "/", "\\");
			if (PathFileExistsA(name.c_str()) != TRUE) {
				DWORD Error = GetLastError();
				if (Error == ERROR_FILE_NOT_FOUND || Error == ERROR_PATH_NOT_FOUND)
				{
					return false; // file does not exist
				}
			}
			return true;
#endif
		}

		void OSVFSProvider::remove(const Path & p)
		{
#if defined(__linux__)
			std::string name = _base + p.getString();
			::remove(name.c_str());
#else
			std::string name = _base + stringReplace(p.getString(), "/", "\\");
			::remove(name.c_str());
#endif
		}

		std::vector<Path> OSVFSProvider::getFiles(const Path & p)
		{
			std::vector<Path> res;
#if defined(__linux__)
			std::string name = _base + p.getString();
			auto deleter = [](DIR* d) { closedir(d); };
			std::unique_ptr<DIR, decltype(deleter)> dir(opendir(name.c_str()), deleter);
			struct dirent data;
			struct dirent* ptr;
			readdir_r(dir.get(), &data, &ptr);
			while (ptr != nullptr)
			{
				if (data.d_type == DT_DIR)
				{
					res.push_back(Path(p.getString() + "/" + data.d_name + std::string("/")));
				}
				else {
					res.push_back(Path(p.getString() + "/" + data.d_name));
				}
				readdir_r(dir.get(), &data, &ptr);
			}
			return res;
#else
			WIN32_FIND_DATAA ffd;
			HANDLE hFind = INVALID_HANDLE_VALUE;

			// Check that the input path plus 3 is not longer than MAX_PATH.
			// Three characters are for the "\*" plus NULL appended below.
			std::string name = _base + stringReplace(p.getString(), "/", "\\");
			if (name.length() + 3 > MAX_PATH)
				throw std::runtime_error("Path to long");
			name += "*";
			// Find the first file in the directory.

			hFind = FindFirstFileA(name.c_str(), &ffd);

			if (INVALID_HANDLE_VALUE == hFind)
			{
				throw std::runtime_error("FindFirstFile failed");
			}
			do
			{
				std::string rp = p.getString() + ffd.cFileName;
				if (ffd.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
					rp += "/";
				res.push_back(Path(rp));
			} while (FindNextFileA(hFind, &ffd) != 0);

			FindClose(hFind);
			return res;
#endif
		}

		InputOutputStreamPtr OSVFSProvider::openIO(const Path & path)
		{
#ifdef __linux__
			std::string name = _base + path.getString();
#else
			std::string name = _base + stringReplace(path.getString(), "/", "\\");
#endif
			return std::make_shared<OSVFSInputOutputStream>(name);
		}

		InputStreamPtr OSVFSProvider::openInput(const Path & path)
		{
#ifdef __linux__
			std::string name = _base + path.getString();
#else
			std::string name = _base + stringReplace(path.getString(), "/", "\\");
#endif
			return std::make_shared<OSVFSInputStream>(name);
		}

		OutputStreamPtr OSVFSProvider::openOutput(const Path & path)
		{
#ifdef __linux__
			std::string name = _base + path.getString();
#else
			std::string name = _base + stringReplace(path.getString(), "/", "\\");
#endif
			return std::make_shared<OSVFSOutputStream>(name);
		}

		std::string OSVFSProvider::getCurrentWorkingDirectory()
		{
#ifdef __linux__
			std::unique_ptr<char> ptr(get_current_dir_name());
			return std::string(ptr.get());
#else
			std::string str;
			str.resize(GetCurrentDirectoryA(0, nullptr));
			GetCurrentDirectoryA(str.size(), (char*)str.data());
			str.resize(str.size() - 1); // remove null char
			return str;
#endif
		}

		void OSVFSProvider::rename(const Path & p, const Path & target)
		{
#if defined(__linux__)
			std::string name1 = _base + p.getString();
			std::string name2 = _base + target.getString();
			::rename(name1.c_str(), name2.c_str());
#else
			std::string name1 = _base + stringReplace(p.getString(), "/", "\\");
			std::string name2 = _base + stringReplace(target.getString(), "/", "\\");
			::rename(name1.c_str(), name2.c_str());
#endif
		}
	}
}
