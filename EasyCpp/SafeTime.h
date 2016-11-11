#pragma once
#include <ctime>
#include "DllExport.h"

namespace EasyCpp
{
	class DLL_EXPORT Time
	{
	public:
		/// <summary>Threadsafe localtime</summary>
		static struct tm localtime(time_t time);
	};
}