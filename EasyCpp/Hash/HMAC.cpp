#include "HMAC.h"
#include "HashManager.h"

namespace EasyCpp
{
	namespace Hash
	{
		HMAC::HMAC(std::string hash)
			: HMAC(HashManager::getHash(hash))
		{
		}

		HMAC::HMAC(HashPtr hash)
			:_hash(hash)
		{
		}

		std::string HMAC::run(const std::string & message, const std::string & key)
		{
			std::string mkey = key;
			auto blocksize = _hash->blocksize();
			if (mkey.length() > blocksize) {
				_hash->update(mkey);
				mkey = _hash->final();
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

			_hash->reset();
			_hash->update(i_key_pad + message);
			std::string inner = _hash->final();
			_hash->reset();
			_hash->update(o_key_pad + inner);
			return _hash->final();
		}
	}
}