#include "SHA1.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <openssl/sha.h>

#pragma comment(lib,"libeay32.lib")

namespace EasyCpp
{
	namespace Hash
	{
		AUTO_INIT({
			HashManager::registerHash("sha1", []() {
				return std::make_shared<SHA1>();
			});
		})

		SHA1::SHA1()
		{
			sha1 = new SHA_CTX();
			SHA1_Init((SHA_CTX*)sha1);
		}

		SHA1::~SHA1()
		{
			delete ((SHA_CTX*)sha1);
		}

		void SHA1::update(const std::string & str)
		{
			SHA1_Update((SHA_CTX*)sha1, str.c_str(), str.size());
		}

		std::string SHA1::final()
		{
			std::string res = std::string(SHA_DIGEST_LENGTH, 0x00);
			SHA1_Final((unsigned char*)res.data(), (SHA_CTX*)sha1);
			return res;
		}

		size_t SHA1::blocksize()
		{
			return SHA_CBLOCK;
		}

		size_t SHA1::outputsize()
		{
			return SHA_DIGEST_LENGTH;
		}

		void SHA1::reset()
		{
			delete ((SHA_CTX*)sha1);
			sha1 = new SHA_CTX();
			SHA1_Init((SHA_CTX*)sha1);
		}

		std::string SHA1::getString(const std::string & str)
		{
			SHA1 sha1;
			sha1.update(str);
			return sha1.final();
		}
	}
}
