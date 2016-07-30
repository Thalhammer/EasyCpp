#pragma once
#include <string>
#include "Severity.h"
#include "../Bundle.h"

namespace EasyCpp
{
	namespace Logging
	{
		class DLL_EXPORT ILogger
		{
		public:
			virtual ~ILogger() {}

			virtual void Emergency(std::string message, Bundle context = {}) = 0;
			virtual void Alert(std::string message, Bundle context = {}) = 0;
			virtual void Critical(std::string message, Bundle context = {}) = 0;
			virtual void Error(std::string message, Bundle context = {}) = 0;
			virtual void Warning(std::string message, Bundle context = {}) = 0;
			virtual void Notice(std::string message, Bundle context = {}) = 0;
			virtual void Info(std::string message, Bundle context = {}) = 0;
			virtual void Debug(std::string message, Bundle context = {}) = 0;

			virtual void Log(Severity severity, std::string message, Bundle context = {}) = 0;
		};
		typedef std::shared_ptr<ILogger> ILoggerPtr;
	}
}
