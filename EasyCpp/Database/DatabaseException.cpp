#include "DatabaseException.h"

namespace EasyCpp
{
	namespace Database
	{
		DatabaseException::DatabaseException(const std::string & code, const Bundle & info)
			: RuntimeException(code), _code(code), _info(info)
		{
		}

		DatabaseException::~DatabaseException()
		{
		}

		const std::string & DatabaseException::getCode() const
		{
			return _code;
		}

		const Bundle & DatabaseException::getInfo() const
		{
			return _info;
		}
	}
}