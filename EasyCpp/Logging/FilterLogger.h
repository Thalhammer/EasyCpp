#pragma once
#include "AbstractLogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		class DLL_EXPORT FilterLogger : public AbstractLogger
		{
		public:
			FilterLogger(ILoggerPtr logger);
			virtual ~FilterLogger();

			// Geerbt über AbstractLogger
			virtual void Log(Severity severity, std::string message, Bundle context) override;

			void enableLevel(Severity severity);
			void disableLevel(Severity severity);
			void setMaxLevel(Severity severity);
		private:
			ILoggerPtr _logger;
			std::map<Severity, bool> _enabled;
		};
	}
}