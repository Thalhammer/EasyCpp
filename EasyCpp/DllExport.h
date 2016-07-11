#pragma once

#if defined(__linux__)
#define DLL_EXPORT 
#elif defined(_WIN32)
#define DLL_EXPORT _declspec(dllexport)
#pragma warning(disable: 4251)
#endif