#include "Program.h"

namespace EasyCpp
{
	Program::Program()
	{
	}

	Program::Program(const std::string & path)
	{
	}

	Program::Program(const std::string & path, const env_t & env)
	{
	}

	Program::Program(const std::string & path, const args_t & args, const env_t & env)
	{
	}

	Program::~Program()
	{
	}

	void Program::open(const std::string & path, const args_t & args, const env_t & env)
	{
	}

	void Program::kill()
	{
	}

	void Program::wait()
	{
	}

	VFS::InputStreamPtr Program::getStdin()
	{
		return VFS::InputStreamPtr();
	}

	VFS::OutputStreamPtr Program::getStdout()
	{
		return VFS::OutputStreamPtr();
	}
}