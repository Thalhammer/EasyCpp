#pragma once
#include "DllExport.h"

namespace EasyCpp
{
	class DLL_EXPORT NonCopyable
	{
	public:
		NonCopyable() {}
		virtual ~NonCopyable() {}
		NonCopyable(const NonCopyable& other) = delete; // non construction-copyable
		NonCopyable& operator=(const NonCopyable&) = delete; // non copyable
	};
}