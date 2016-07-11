#include "ConvertException.h"

namespace EasyCpp
{
	ConvertException::ConvertException(const std::string & what)
		:RuntimeException(what)
	{
	}

	ConvertException::~ConvertException()
	{
	}
}