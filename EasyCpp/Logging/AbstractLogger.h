#pragma once
#include "ILogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		class AbstractLogger : public ILogger
		{
		public:
			// Geerbt über ILogger
			virtual void Emergency(std::string message, Bundle context) override;
			virtual void Alert(std::string message, Bundle context) override;
			virtual void Critical(std::string message, Bundle context) override;
			virtual void Error(std::string message, Bundle context) override;
			virtual void Warning(std::string message, Bundle context) override;
			virtual void Notice(std::string message, Bundle context) override;
			virtual void Info(std::string message, Bundle context) override;
			virtual void Debug(std::string message, Bundle context) override;
		};
	}
}