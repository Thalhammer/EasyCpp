#include "AbstractLogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		AbstractLogger::~AbstractLogger()
		{
		}

		void AbstractLogger::Emergency(std::string message, Bundle context)
		{
			this->Log(Severity::Emergency, message, context);
		}

		void AbstractLogger::Alert(std::string message, Bundle context)
		{
			this->Log(Severity::Alert, message, context);
		}

		void AbstractLogger::Critical(std::string message, Bundle context)
		{
			this->Log(Severity::Critical, message, context);
		}

		void AbstractLogger::Error(std::string message, Bundle context)
		{
			this->Log(Severity::Error, message, context);
		}

		void AbstractLogger::Warning(std::string message, Bundle context)
		{
			this->Log(Severity::Warning, message, context);
		}

		void AbstractLogger::Notice(std::string message, Bundle context)
		{
			this->Log(Severity::Notice, message, context);
		}

		void AbstractLogger::Info(std::string message, Bundle context)
		{
			this->Log(Severity::Informational, message, context);
		}

		void AbstractLogger::Debug(std::string message, Bundle context)
		{
			this->Log(Severity::Debug, message, context);
		}
	}
}