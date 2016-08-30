#include "FilterLogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		FilterLogger::FilterLogger(ILoggerPtr logger)
			: _logger(logger)
		{
			this->setMaxLevel(Severity::MAX);
		}

		FilterLogger::~FilterLogger()
		{
		}

		void FilterLogger::Log(Severity severity, std::string message, Bundle context)
		{
			if (_enabled[severity]) {
				_logger->Log(severity, message, context);
			}
		}

		void FilterLogger::enableLevel(Severity severity)
		{
			_enabled[severity] = true;
		}

		void FilterLogger::disableLevel(Severity severity)
		{
			_enabled[severity] = false;
		}

		void FilterLogger::setMaxLevel(Severity severity)
		{
			for (int i = (int)Severity::MIN; i <= (int)severity; i++) {
				this->enableLevel((Severity)i);
			}
			for (int i = (int)severity + 1; i < (int)Severity::MAX + 1; i++) {
				this->disableLevel((Severity)i);
			}
		}
	}
}