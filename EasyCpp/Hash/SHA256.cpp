#include "SHA256.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <openssl/sha.h>

#pragma comment(lib,"libeay32.lib")

namespace EasyCpp
{
	namespace Hash
	{
		AUTO_INIT({
			HashManager::registerHash("sha256", []() {
				return std::make_shared<SHA256>();
			});
		})

		SHA256::SHA256()
		{
			sha256 = new SHA256_CTX();
			SHA256_Init((SHA256_CTX*)sha256);
		}

		SHA256::~SHA256()
		{
			delete ((SHA256_CTX*)sha256);
		}

		void SHA256::update(const std::string & str)
		{
			SHA256_Update((SHA256_CTX*)sha256, str.c_str(), str.size());
		}

		std::string SHA256::final()
		{
			std::string res = std::string(SHA256_DIGEST_LENGTH, 0x00);
			SHA256_Final((unsigned char*)res.data(), (SHA256_CTX*)sha256);
			return res;
		}

		size_t EasyCpp::Hash::SHA256::blocksize()
		{
			return SHA256_CBLOCK;
		}

		size_t SHA256::outputsize()
		{
			return SHA256_DIGEST_LENGTH;
		}

		void SHA256::reset()
		{
			delete ((SHA256_CTX*)sha256);
			sha256 = new SHA256_CTX();
			SHA256_Init((SHA256_CTX*)sha256);
		}

		std::string SHA256::getString(const std::string & str)
		{
			SHA256 sha256;
			sha256.update(str);
			return sha256.final();
		}
	}
}