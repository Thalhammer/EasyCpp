#include "SHA384.h"
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
			unsigned char hash[SHA384_DIGEST_LENGTH];
			SHA384_Final(hash, (SHA512_CTX*)sha384);
			std::stringstream ss;
			for (int i = 0; i < SHA384_DIGEST_LENGTH; i++)
			{
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
			}
			return ss.str();
		}

		size_t EasyCpp::Hash::SHA384::blocksize()
		{
			return SHA512_CBLOCK;
		}

		std::string SHA384::getString(const std::string & str)
		{
			SHA384 sha384;
			sha384.update(str);
			return sha384.final();
		}
	}
}