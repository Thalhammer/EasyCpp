#ifdef __linux__
#include "SystemLogger.h"
#include "../RuntimeException.h"
#include "../StringAlgorithm.h"
#include <syslog.h>

namespace EasyCpp
{
	namespace Logging
	{
		SystemLogger::SystemLogger(const std::string & source)
		{
			openlog(source.c_str(), LOG_PID, LOG_USER);
			_handle = std::shared_ptr<void>(nullptr, []() {
				closelog();
			});
		}

		SystemLogger::~SystemLogger()
		{
		}

		void SystemLogger::Log(Severity severity, std::string message, Bundle context)
		{
			int priority = LOG_EMERG;
			switch (severity) {
			case Severity::Debug:
				priority = LOG_DEBUG; break;
			case Severity::Informational:
				priority = LOG_INFO; break;
			case Severity::Notice:
				priority = LOG_NOTICE; break;
			case Severity::Warning:
				priority = LOG_WARNING; break;
			case Severity::Alert:
				priority = LOG_ALERT; break;
			case Severity::Critical:
				priority = LOG_CRIT; break;
			case Severity::Emergency:
				priority = LOG_EMERG; break;
			case Severity::Error:
				priority = LOG_ERR; break;
			}

			for (auto& elem : context) {
				message = replaceAll(message, "{" + elem.first + "}", elem.second.as<std::string>());
			}

			syslog(priority, message.c_str());
		}
	}
}

#endif