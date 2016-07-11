#include "LuaException.h"

namespace EasyCpp
{
	namespace Scripting
	{

		LuaException::LuaException(const std::string& msg)
			:_what(msg)
		{
		}


		LuaException::~LuaException()
		{
		}

		const char * LuaException::what() const noexcept
		{
			return (std::string("LuaException(") + _what + ")").c_str();
		}

	}
}