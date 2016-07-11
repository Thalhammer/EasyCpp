#include "RuntimeException.h"

namespace EasyCpp
{
	RuntimeException::RuntimeException(const std::string & what)
		:_what(what)
	{
	}

	RuntimeException::~RuntimeException()
	{
	}

	const char * RuntimeException::what() const noexcept
	{
		return _what.c_str();
	}
}