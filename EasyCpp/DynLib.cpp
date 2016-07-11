#include "DynLib.h"
#include <stdexcept>
#if defined(__linux__)
#include <dlfcn.h>
#elif defined(_WIN32)
#include <windows.h>
#include <codecvt>
#endif

namespace EasyCpp
{
	DynLib::DynLib(const std::string& file)
	{
#if defined(__linux__)
		_handle = std::shared_ptr<void>(
			dlopen(file.c_str(), RTLD_NOW),
			[](void* ptr) { if (ptr != nullptr) dlclose(ptr); });
#elif defined(_WIN32)
		std::wstring_convert<std::codecvt_utf8_utf16<uint16_t>, uint16_t> convert;
		std::basic_string<uint16_t> wfile = convert.from_bytes(file);
		_handle = std::shared_ptr<void>(
			(void*)LoadLibrary((LPCWSTR)wfile.c_str()),
			[](void* ptr) { FreeLibrary((HMODULE)ptr); });
#else
#pragma error DynLib is not implemented for this platform.
#endif
		if (_handle == nullptr) {
			throwError();
		}
	}

	DynLib::~DynLib()
	{
	}

	void* DynLib::getFunction(const std::string& fn)
	{
#if defined(__linux__)
		void* res = dlsym(_handle.get(), fn.c_str());
		if (res == nullptr)
			throwError();
		return res;
#elif defined(_WIN32)
		void* res = (void*)GetProcAddress((HMODULE)(_handle.get()), fn.c_str());
		if (res == nullptr)
			throwError();
		return res;
#else
		return nullptr;
#endif
	}

	void DynLib::throwError()
	{
#if defined(__linux__)
		throw std::runtime_error(dlerror());
#elif defined(_WIN32)
		throw std::runtime_error(std::to_string(GetLastError()));
#endif
	}

}