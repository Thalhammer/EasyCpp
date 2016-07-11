#pragma once
#include "RuntimeException.h"

namespace EasyCpp
{
	class DLL_EXPORT ConvertException :
		public RuntimeException
	{
	public:
		ConvertException(const std::string& what);
		virtual ~ConvertException();
	};
}