#include "LuaException.h"

namespace EasyCpp
{
	namespace Scripting
	{

		LuaException::LuaException(const std::string& msg)
			:_what("LuaException(" + msg + ")")
		{
		}


		LuaException::~LuaException()
		{
		}

		const char * LuaException::what() const noexcept
		{
			return _what.c_str();
		}

	}
}