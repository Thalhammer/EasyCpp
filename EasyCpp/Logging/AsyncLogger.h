#pragma once
#include "AbstractLogger.h"
#include "../Queue.h"
#include <atomic>

namespace EasyCpp
{
	namespace Logging
	{
		class DLL_EXPORT AsyncLogger : public AbstractLogger
		{
		public:
			AsyncLogger(ILoggerPtr logger);
			virtual ~AsyncLogger();

			// Geerbt über AbstractLogger
			virtual void Log(Severity severity, std::string message, Bundle context) override;
		private:
			ILoggerPtr _logger;
			typedef std::tuple<Severity, std::string, Bundle> Entry_t;
			std::queue<Entry_t> _queue;
			std::mutex _mutex;
			std::condition_variable _cv;

			std::atomic_bool _exit_thread;
			std::thread _thread;
		};
	}
}