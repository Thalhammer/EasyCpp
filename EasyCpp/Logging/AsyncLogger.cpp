#include "AsyncLogger.h"

namespace EasyCpp
{
	namespace Logging
	{
		AsyncLogger::AsyncLogger(ILoggerPtr logger)
			: _logger(logger)
		{
			_exit_thread.store(false);
			_thread = std::thread([this]() {
				while (true) {
					Entry_t item;
					{
						std::unique_lock<std::mutex> lck(_mutex);
						while (_queue.empty()) {
							if (_exit_thread.load())
								return;
							_cv.wait(lck);
						}
						item = _queue.front();
						_queue.pop();
					}
					_logger->Log(std::get<0>(item), std::get<1>(item), std::get<2>(item));
				}
			});
		}

		AsyncLogger::~AsyncLogger()
		{
			_exit_thread.store(true);
			_cv.notify_all();
			_thread.join();
		}

		void AsyncLogger::Log(Severity severity, std::string message, Bundle context)
		{
			std::unique_lock<std::mutex> lck(_mutex);
			_queue.push(Entry_t(severity, message, context));
			lck.unlock();
			_cv.notify_one();
		}
	}
}
