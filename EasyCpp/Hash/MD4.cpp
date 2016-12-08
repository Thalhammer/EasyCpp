#include "MD4.h"
#include "HashManager.h"
#include "../AutoInit.h"
#include <openssl/md4.h>
#include "../HexEncoding.h"

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
			std::string res = std::string(MD4_DIGEST_LENGTH, 0x00);
			MD4_Final((unsigned char*)res.data(), (MD4_CTX*)md4);
			return res;
		}

		size_t MD4::blocksize()
		{
			return MD4_CBLOCK;
		}

		size_t MD4::outputsize()
		{
			return MD4_DIGEST_LENGTH;
		}

		void MD4::reset()
		{
			delete ((MD4_CTX*)md4);
			md4 = new MD4_CTX();
			MD4_Init((MD4_CTX*)md4);
		}

		std::string MD4::getString(const std::string & str)
		{
			MD4 md4;
			md4.update(str);
			return md4.final();
		}
		
	}
}