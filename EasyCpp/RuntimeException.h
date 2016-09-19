#pragma once
#include "BasicException.h"

namespace EasyCpp
{
	/// <summary>A runtime exception with backtrace.</summary>
	class DLL_EXPORT RuntimeException :
		public BasicException
	{
	public:
		explicit RuntimeException(const std::string& what);
		virtual ~RuntimeException();
		virtual const char* what() const noexcept;
	private:
		std::string _what;
	};
}
