#ifdef _WIN32
#include "SystemLogger.h"
#include "../RuntimeException.h"
#include "../StringAlgorithm.h"
#include <Windows.h>

namespace EasyCpp
{
	namespace Logging
	{
		SystemLogger::SystemLogger(const std::string & source)
			:_source(source)
		{
			_handle = std::shared_ptr<void>(RegisterEventSourceA(NULL, source.c_str()), [](void* handle) {
				if (handle != nullptr) {
					DeregisterEventSource(handle);
				}
			});
			if (!_handle) {
				throw RuntimeException("Failed to register event source");
			}
		}

		SystemLogger::~SystemLogger()
		{
		}

		void SystemLogger::Log(Severity severity, std::string message, Bundle context)
		{
			WORD type = EVENTLOG_ERROR_TYPE;
			switch (severity) {
			case Severity::Debug:
			case Severity::Informational:
			case Severity::Notice:
				type = EVENTLOG_INFORMATION_TYPE; break;
			case Severity::Warning:
			case Severity::Alert:
				type = EVENTLOG_WARNING_TYPE; break;
			case Severity::Critical:
			case Severity::Emergency:
			case Severity::Error:
				type = EVENTLOG_ERROR_TYPE; break;
			}

			for (auto& elem : context) {
				message = replaceAll(message, "{" + elem.first + "}", elem.second.as<std::string>());
			}

			auto wstr = ValueConverter::convert<std::string, std::wstring>(message);
			LPCTSTR strings[] = { wstr.c_str()};
			auto res = ReportEvent(_handle.get(), type, 0, 0, NULL, 1, 0, strings, NULL);
			if (res == 0) {
				throw RuntimeException("Failed to log event");
			}
		}
	}
}

#endif