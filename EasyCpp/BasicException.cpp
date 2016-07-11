#include "BasicException.h"

namespace EasyCpp
{
	BasicException::BasicException() noexcept
	{
	}


	BasicException::~BasicException()
	{
	}

	const char * BasicException::what() const throw()
	{
		return "BasicException";
	}

	const Backtrace & BasicException::backtrace() const
	{
		return _trace;
	}
}