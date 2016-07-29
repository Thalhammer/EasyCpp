#include "SHA512.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <openssl/sha.h>
#include "../HexEncoding.h"

#pragma comment(lib,"libeay32.lib")

namespace EasyCpp
{
	namespace Hash
	{
		AUTO_INIT({
			HashManager::registerHash("sha512", []() {
				return std::make_shared<SHA512>();
			});
		})

		SHA512::SHA512()
		{
			sha512 = new SHA512_CTX();
			SHA512_Init((SHA512_CTX*)sha512);
		}

		SHA512::~SHA512()
		{
			delete ((SHA512_CTX*)sha512);
		}

		void SHA512::update(const std::string & str)
		{
			SHA512_Update((SHA512_CTX*)sha512, str.c_str(), str.size());
		}

		std::string SHA512::final()
		{
			std::string res = std::string(SHA512_DIGEST_LENGTH, 0x00);
			SHA512_Final((unsigned char*)res.data(), (SHA512_CTX*)sha512);
			return HexEncoding::encode(res);
		}

		size_t EasyCpp::Hash::SHA512::blocksize()
		{
			return SHA512_CBLOCK;
		}

		void SHA512::reset()
		{
			delete ((SHA512_CTX*)sha512);
			sha512 = new SHA512_CTX();
			SHA512_Init((SHA512_CTX*)sha512);
		}

		std::string SHA512::getString(const std::string & str)
		{
			SHA512 sha512;
			sha512.update(str);
			return sha512.final();
		}
	}
}