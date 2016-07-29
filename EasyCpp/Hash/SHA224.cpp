#include "SHA224.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <sstream>
#include <iomanip>
#include <openssl/sha.h>

#pragma comment(lib,"libeay32.lib")

namespace EasyCpp
{
	namespace Hash
	{
		AUTO_INIT({
			HashManager::registerHash("sha224", []() {
				return std::make_shared<SHA224>();
			});
		})

		SHA224::SHA224()
		{
			sha224 = new SHA256_CTX();
			SHA224_Init((SHA256_CTX*)sha224);
		}

		SHA224::~SHA224()
		{
			delete ((SHA256_CTX*)sha224);
		}

		void SHA224::update(const std::string & str)
		{
			SHA224_Update((SHA256_CTX*)sha224, str.c_str(), str.size());
		}

		std::string SHA224::final()
		{
			unsigned char hash[SHA224_DIGEST_LENGTH];
			SHA224_Final(hash, (SHA256_CTX*)sha224);
			std::stringstream ss;
			for (int i = 0; i < SHA224_DIGEST_LENGTH; i++)
			{
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
			}
			return ss.str();
		}

		size_t EasyCpp::Hash::SHA224::blocksize()
		{
			return SHA256_CBLOCK;
		}

		std::string SHA224::getString(const std::string & str)
		{
			SHA224 sha224;
			sha224.update(str);
			return sha224.final();
		}
	}
}