#include "PBKDF1.h"
#include "HashManager.h"
#include <stdexcept>

namespace EasyCpp
{
	namespace Hash
	{
		PBKDF1::PBKDF1(std::string hash)
			: PBKDF1(HashManager::getHash(hash))
		{
		}

		PBKDF1::PBKDF1(HashPtr hash)
			: _hash(hash)
		{
		}

		std::string PBKDF1::run(const std::string & password, const std::string & salt, size_t num_iterations, size_t num_bytes)
		{
			if (num_bytes == 0)
				num_bytes = _hash->outputsize();
			if (num_bytes > _hash->outputsize())
				throw std::invalid_argument("derived key too long");

			std::string out = password + salt;
			for (size_t i = 0; i < num_iterations; i++)
			{
				_hash->update(out);
				out = _hash->final();
				_hash->reset();
			}
			return out.substr(0, num_bytes);
		}
	}
}