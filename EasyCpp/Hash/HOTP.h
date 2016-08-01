#pragma once
#include <string>
#include "../DllExport.h"

namespace EasyCpp
{
	namespace Hash
	{
		/// <summary>HMAC One Time Password generator.</summary>
		class DLL_EXPORT HOTP
		{
		public:
			/// <summary>Instanciate HOTP with given key.</summary>
			HOTP(const std::string& key);

			/// <summary>Generate a token using the given counter and generate num_digits length.</summary>
			std::string generate(uint64_t counter, size_t num_digits = 6);
		private:
			std::string _key;
		};
	}
}