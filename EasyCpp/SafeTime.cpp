#include "SafeTime.h"

namespace EasyCpp
{
	tm Time::localtime(time_t time)
	{
		struct tm result;
#ifdef _WIN32
		localtime_s(&result, &time);
#elif defined(__linux__)
		localtime_r(&time, &result);
#else
#pragma warning No threadsafe localtime on this plattform
		result = *localtime(&time);
#endif
		return result;
	}
}