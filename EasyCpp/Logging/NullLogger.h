#pragma once
#include "AbstractLogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		class NullLogger : public AbstractLogger
		{
		public:
			// Geerbt �ber AbstractLogger
			virtual void Log(Severity severity, std::string message, Bundle context) override;
		};
	}
}