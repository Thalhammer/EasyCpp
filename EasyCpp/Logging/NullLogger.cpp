#include "NullLogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		void NullLogger::Log(Severity severity, std::string message, Bundle context)
		{
		}
	}
}