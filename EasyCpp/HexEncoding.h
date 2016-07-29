#pragma once
#include <string>
#include "DllExport.h"

namespace EasyCpp
{
	class DLL_EXPORT HexEncoding
	{
	public:
		static std::string encode(const std::string& str);
		static std::string decode(const std::string& hex);
	};
}