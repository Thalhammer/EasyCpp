#include "SHA1.h"
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
			HashManager::registerHash("sha1", []() {
				return std::make_shared<SHA1>();
			});
		})

		SHA1::SHA1()
		{
			sha1 = new SHA_CTX();
			int i= SHA1_Init((SHA_CTX*)sha1);
		}

		SHA1::~SHA1()
		{
			delete ((SHA_CTX*)sha1);
		}

		void SHA1::update(const std::string & str)
		{
			int i = SHA1_Update((SHA_CTX*)sha1, str.c_str(), str.size());
		}

		std::string SHA1::final()
		{
			unsigned char hash[SHA_DIGEST_LENGTH];
			int i = SHA1_Final(hash, (SHA_CTX*)sha1);
			std::stringstream ss;
			for (int i = 0; i < SHA_DIGEST_LENGTH; i++)
			{
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
			}
			return ss.str();
		}

		std::string SHA1::getString(const std::string & str)
		{
			SHA1 sha1;
			sha1.update(str);
			return sha1.final();
		}
	}
}