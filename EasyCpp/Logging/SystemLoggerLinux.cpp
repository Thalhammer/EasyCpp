#ifdef __linux__
#include "SystemLogger.h"
#include "../RuntimeException.h"
#include "../StringAlgorithm.h"
#include <syslog.h>
#include <mutex>

namespace EasyCpp
{
	namespace Logging
	{
		SystemLogger::SystemLogger(const std::string & source)
			:_source(source)
		{
		}

		SystemLogger::~SystemLogger()
		{
		}

		void SystemLogger::Log(Severity severity, std::string message, Bundle context)
		{
			static std::mutex _log_mtx;
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

			{
				std::lock_guard<std::mutex> lck(_log_mtx);
				openlog(_source.c_str(), LOG_PID, LOG_USER);
				syslog(priority, "%s", message.c_str());
				closelog();
			}
		}
	}
}

#endif