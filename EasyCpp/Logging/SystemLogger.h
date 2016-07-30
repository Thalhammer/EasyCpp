#pragma once
#include "AbstractLogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		class DLL_EXPORT SystemLogger : public AbstractLogger
		{
		public:
			SystemLogger(const std::string& source);
			virtual ~SystemLogger();

			// Geerbt über AbstractLogger
			virtual void Log(Severity severity, std::string message, Bundle context) override;
		private:
			std::shared_ptr<void> _handle;
		};
	}
}