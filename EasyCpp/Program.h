#pragma once
#include <string>
#include <map>
#include <vector>
#include "DllExport.h"

#include "VFS/InputStream.h"
#include "VFS/OutputStream.h"

namespace EasyCpp
{
	class DLL_EXPORT Program
	{
	public:
		typedef std::vector<std::string> args_t;
		typedef std::map<std::string, std::string> env_t;
		Program();
		Program(const std::string& path);
		Program(const std::string& path, const env_t& env);
		Program(const std::string& path, const args_t& args, const env_t& env = {});
		virtual ~Program();

		void open(const std::string& path, const args_t& args, const env_t& env);
		void kill();
		void wait();
		bool isAlive();

		VFS::OutputStreamPtr getStdin();
		VFS::InputStreamPtr getStdout();
		VFS::InputStreamPtr getStderr();

		uint32_t getExitCode() const;
		uint64_t getHandle() const;
	private:
		class Impl;
		std::unique_ptr<Impl> _impl;
	};
}