#include "SHA384.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <openssl/sha.h>

#pragma comment(lib,"libeay32.lib")

namespace EasyCpp
{
	namespace Hash
	{
		AUTO_INIT({
			HashManager::registerHash("sha384", []() {
				return std::make_shared<SHA384>();
			});
		})

		SHA384::SHA384()
		{
			sha384 = new SHA512_CTX();
			SHA384_Init((SHA512_CTX*)sha384);
		}

		SHA384::~SHA384()
		{
			delete ((SHA512_CTX*)sha384);
		}

		void SHA384::update(const std::string & str)
		{
			SHA384_Update((SHA512_CTX*)sha384, str.c_str(), str.size());
		}

		std::string SHA384::final()
		{
			std::string res = std::string(SHA384_DIGEST_LENGTH, 0x00);
			SHA384_Final((unsigned char*)res.data(), (SHA512_CTX*)sha384);
			return res;
		}

		size_t EasyCpp::Hash::SHA384::blocksize()
		{
			return SHA512_CBLOCK;
		}

		size_t SHA384::outputsize()
		{
			return SHA384_DIGEST_LENGTH;
		}

		void SHA384::reset()
		{
			delete ((SHA512_CTX*)sha384);
			sha384 = new SHA512_CTX();
			SHA384_Init((SHA512_CTX*)sha384);
		}

		std::string SHA384::getString(const std::string & str)
		{
			SHA384 sha384;
			sha384.update(str);
			return sha384.final();
		}
	}
}