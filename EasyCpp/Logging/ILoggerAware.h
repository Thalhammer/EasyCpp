#pragma once
#include "ILogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		class ILoggerAware
		{
		public:
			virtual void setLogger(ILoggerPtr logger) = 0;
		};
	}
}