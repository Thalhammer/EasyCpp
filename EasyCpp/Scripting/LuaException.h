#pragma once
#include "../BasicException.h"
#include <string>
namespace EasyCpp
{
	namespace Scripting
	{
		class DLL_EXPORT LuaException : public BasicException
		{
		public:
			LuaException(const std::string& msg);
			~LuaException();

			virtual const char* what() const noexcept;
		private:
			std::string _what;
		};
	}
}
