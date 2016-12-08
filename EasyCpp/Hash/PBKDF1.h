#pragma once
#include "Hash.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT PBKDF1
		{
		public:
			/// <summary>Instanciate a PBKDF2 using the named hash.</summary>
			PBKDF1(std::string hash = "sha1");
			/// <summary>Instanciate a PBKDF2 using the gived hash engine.</summary>
			PBKDF1(HashPtr hash);

			/// <summary>Generate PBKDF2 value using the given values.</summary>
			std::string run(const std::string& password, const std::string& salt, size_t num_iterations = 1000, size_t num_bytes = 0);
		private:
			HashPtr _hash;
		};
	}
}