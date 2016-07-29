#pragma once
#include "Hash.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT HMAC
		{
		public:
			HMAC(std::string hash);
			HMAC(HashPtr hash);

			std::string run(const std::string& message, const std::string& key);
		private:
			HashPtr _hash;
		};
	}
}