#include "MD5.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <sstream>
#include <iomanip>
#include <openssl/md5.h>

#pragma comment(lib,"libeay32.lib")

namespace EasyCpp
{
	namespace Hash
	{
		AUTO_INIT({
			HashManager::registerHash("md5", []() {
				return std::make_shared<MD5>();
			});
		})

		MD5::MD5()
		{
			md5 = new MD5_CTX();
			MD5_Init((MD5_CTX*)md5);
		}

		MD5::~MD5()
		{
			delete ((MD5_CTX*)md5);
		}

		void MD5::update(const std::string & str)
		{
			MD5_Update((MD5_CTX*)md5, str.c_str(), str.size());
		}

		std::string MD5::final()
		{
			unsigned char hash[MD5_DIGEST_LENGTH];
			MD5_Final(hash, (MD5_CTX*)md5);
			std::stringstream ss;
			for (int i = 0; i < MD5_DIGEST_LENGTH; i++)
			{
				ss << std::hex << std::setw(2) << std::setfill('0') << (int)hash[i];
			}
			return ss.str();
		}

		std::string MD5::getString(const std::string & str)
		{
			MD5 md5;
			md5.update(str);
			return md5.final();
		}
	}
}