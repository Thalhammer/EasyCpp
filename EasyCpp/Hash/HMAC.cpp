#include "HMAC.h"
#include "HashManager.h"
#include "../HexEncoding.h"

namespace EasyCpp
{
	namespace Hash
	{
		HMAC::HMAC(std::string hash)
			:_hash(hash)
		{
		}

		std::string HMAC::run(const std::string & message, const std::string & key)
		{
			auto hash = HashManager::getHash(_hash);
			std::string mkey = key;
			auto blocksize = hash->blocksize();
			if (mkey.length() > blocksize) {
				hash->update(mkey);
				mkey = HexEncoding::decode(hash->final());
			}
			if (mkey.length() < blocksize) {
				mkey = mkey + std::string((blocksize - mkey.length()), 0x00);
			}
			std::string o_key_pad = std::string(blocksize, 0x5c);
			std::string i_key_pad = std::string(blocksize, 0x36);
			for (size_t i = 0; i < blocksize; i++) {
				o_key_pad[i] = o_key_pad[i] ^ mkey[i];
				i_key_pad[i] = i_key_pad[i] ^ mkey[i];
			}

			hash = HashManager::getHash(_hash);
			hash->update(i_key_pad + message);
			std::string inner = HexEncoding::decode(hash->final());
			hash = HashManager::getHash(_hash);
			hash->update(o_key_pad + inner);
			return hash->final();
		}
	}
}