#pragma once
#include "RuntimeException.h"

namespace EasyCpp
{
	/// <summary>Exception which gets thrown when ValueConverter fails.</summary>
	class DLL_EXPORT ConvertException :
		public RuntimeException
	{
	public:
		ConvertException(const std::string& what);
		virtual ~ConvertException();
	};
}