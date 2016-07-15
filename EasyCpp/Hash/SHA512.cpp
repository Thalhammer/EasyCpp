#include "SHA512.h"
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
			unsigned char hash[SHA512_DIGEST_LENGTH];
			SHA512_Final(hash, (SHA512_CTX*)sha512);
			std::stringstream ss;
			for (int i = 0; i < SHA512_DIGEST_LENGTH; i++)
			{
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
			}
			return ss.str();
		}

		std::string SHA512::getString(const std::string & str)
		{
			SHA512 sha512;
			sha512.update(str);
			return sha512.final();
		}
	}
}