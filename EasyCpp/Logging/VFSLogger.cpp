#include "VFSLogger.h"
#include "../SafeTime.h"
#include <chrono>
#include <sstream>
#include <iomanip>
#include <iostream>

namespace EasyCpp
{
	namespace Logging
	{
		VFSLogger::VFSLogger(const std::string & source, VFS::OutputStreamPtr stream)
			:_source(source), _stream(stream)
		{
		}

		VFSLogger::~VFSLogger()
		{
		}

		void VFSLogger::Log(Severity severity, std::string message, Bundle context)
		{
			struct tm time = Time::localtime(std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()));
			std::stringstream line;
			line << "["
				<< std::setfill('0') << std::setw(2) << time.tm_mday << "."
				<< std::setfill('0') << std::setw(2) << (time.tm_mon + 1) << "."
				<< std::setfill('0') << std::setw(4) << (time.tm_year + 1900) << " "
				<< std::setfill('0') << std::setw(2) << time.tm_hour << ":"
				<< std::setfill('0') << std::setw(2) << time.tm_min << ":"
				<< std::setfill('0') << std::setw(2) << time.tm_sec << "]";
			std::string str_severity;
			switch (severity) {
			case Severity::Debug:
				str_severity = "DEBUG    "; break;
			case Severity::Informational:
				str_severity = "INFO     "; break;
			case Severity::Notice:
				str_severity = "NOTICE   "; break;
			case Severity::Warning:
				str_severity = "WARNING  "; break;
			case Severity::Alert:
				str_severity = "ALERT    "; break;
			case Severity::Critical:
				str_severity = "CRITICAL "; break;
			case Severity::Emergency:
				str_severity = "EMERGENCY"; break;
			case Severity::Error:
			default:
				str_severity = "ERROR    "; break;
			}

			line << "[" << str_severity << "]";
			line << "[" << _source << "]";
			line << message << std::endl;
			auto temp = line.str();
			{
				(*_stream)->write(std::vector<uint8_t>(temp.begin(), temp.end()));
			}
		}
	}
}
