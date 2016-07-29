#pragma once
#include <string>
#include "../DllExport.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT HOTP
		{
		public:
			HOTP(const std::string& key);

			std::string generate(uint64_t counter, size_t num_digits = 6);
		private:
			std::string _key;
		};
	}
}