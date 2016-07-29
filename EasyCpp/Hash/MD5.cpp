#include "MD5.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <openssl/md5.h>
#include "../HexEncoding.h"

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
			std::string res = std::string(MD5_DIGEST_LENGTH, 0x00);
			MD5_Final((unsigned char*)res.data(), (MD5_CTX*)md5);
			return HexEncoding::encode(res);
		}

		size_t MD5::blocksize()
		{
			return MD5_CBLOCK;
		}

		std::string MD5::getString(const std::string & str)
		{
			MD5 md5;
			md5.update(str);
			return md5.final();
		}
	}
}