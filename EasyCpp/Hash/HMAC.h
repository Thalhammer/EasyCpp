#pragma once
#include "Hash.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT HMAC
		{
		public:
			/// <summary>Instanciate a HMAC using the named hash.</summary>
			HMAC(std::string hash);
			/// <summary>Instanciate a HMAC using the gived hash engine.</summary>
			HMAC(HashPtr hash);

			/// <summary>Generate HMAC value using the given message and key.</summary>
			std::string run(const std::string& message, const std::string& key);
		private:
			HashPtr _hash;
		};
	}
}