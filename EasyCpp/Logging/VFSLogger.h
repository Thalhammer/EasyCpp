#pragma once
#include "AbstractLogger.h"
#include "../VFS/OutputStream.h"
#include "../ThreadSafe.h"
#include <mutex>

namespace EasyCpp
{
	namespace Logging
	{
		class DLL_EXPORT VFSLogger : public AbstractLogger
		{
		public:
			VFSLogger(const std::string& source, VFS::OutputStreamPtr stream);
			virtual ~VFSLogger();

			// Geerbt über AbstractLogger
			virtual void Log(Severity severity, std::string message, Bundle context) override;
		private:
			std::string _source;
			ThreadSafe<VFS::OutputStreamPtr> _stream;
		};
	}
}