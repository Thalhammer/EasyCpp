#include "MD4.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <sstream>
#include <iomanip>
#include <openssl/md4.h>

#pragma comment(lib,"libeay32.lib")

namespace EasyCpp
{
	namespace Hash
	{
		AUTO_INIT({
			HashManager::registerHash("md4", []() {
				return std::make_shared<MD4>();
			});
		})

		MD4::MD4()
		{
			md4 = new MD4_CTX();
			MD4_Init((MD4_CTX*)md4);
		}

		MD4::~MD4()
		{
			delete ((MD4_CTX*)md4);
		}

		void MD4::update(const std::string & str)
		{
			MD4_Update((MD4_CTX*)md4, str.c_str(), str.size());
		}

		std::string MD4::final()
		{
			unsigned char hash[MD4_DIGEST_LENGTH];
			MD4_Final(hash, (MD4_CTX*)md4);
			std::stringstream ss;
			for (int i = 0; i < MD4_DIGEST_LENGTH; i++)
			{
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
			}
			return ss.str();
		}

		size_t MD4::blocksize()
		{
			return MD4_CBLOCK;
		}

		std::string MD4::getString(const std::string & str)
		{
			MD4 md4;
			md4.update(str);
			return md4.final();
		}
		
	}
}