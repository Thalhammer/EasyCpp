#pragma once
#include <ctime>

namespace EasyCpp
{
	class Time
	{
	public:
		/// <summary>Threadsafe localtime</summary>
		static struct tm localtime(time_t time);
	};
}