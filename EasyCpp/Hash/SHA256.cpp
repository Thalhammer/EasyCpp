#include "SHA256.h"
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
			unsigned char hash[SHA256_DIGEST_LENGTH];
			SHA256_Final(hash, (SHA256_CTX*)sha256);
			std::stringstream ss;
			for (int i = 0; i < SHA256_DIGEST_LENGTH; i++)
			{
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
			}
			return ss.str();
		}

		std::string SHA256::getString(const std::string & str)
		{
			SHA256 sha256;
			sha256.update(str);
			return sha256.final();
		}
	}
}