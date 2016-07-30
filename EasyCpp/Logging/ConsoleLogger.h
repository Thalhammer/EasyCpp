#pragma once
#include "AbstractLogger.h"
#include <mutex>

namespace EasyCpp
{
	namespace Logging
	{
		class DLL_EXPORT ConsoleLogger : public AbstractLogger
		{
		public:
			ConsoleLogger(const std::string& source);
			virtual ~ConsoleLogger();

			// Geerbt über AbstractLogger
			virtual void Log(Severity severity, std::string message, Bundle context) override;
		private:
			std::string _source;
			static std::mutex _console_mutex;
		};
	}
}