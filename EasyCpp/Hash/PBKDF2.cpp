#include "PBKDF2.h"
#include "HashManager.h"
#include "HMAC.h"
#include <limits>
#include <stdexcept>

namespace EasyCpp
{
	namespace Hash
	{
		PBKDF2::PBKDF2(std::string hash)
			: PBKDF2(HashManager::getHash(hash))
		{
		}

		PBKDF2::PBKDF2(HashPtr hash)
			: _hash(hash)
		{
		}

		std::string PBKDF2::run(const std::string & password, const std::string & salt, size_t num_iterations, size_t num_bytes)
		{
			if (num_bytes == 0)
				num_bytes = _hash->outputsize();

			if(num_bytes > std::numeric_limits<uint32_t>::max()*_hash->outputsize())
				throw std::invalid_argument("derived key too long");

			HMAC hmac(_hash);

			auto F = [&password, &salt, num_iterations, &hmac](uint32_t index) {
				uint8_t* sindex = (uint8_t*)&index;
				std::string t(4, 0x00);
				t[0] = sindex[3];
				t[1] = sindex[2];
				t[2] = sindex[1];
				t[3] = sindex[0];
				std::string u = salt + t;

				std::string out = hmac.run(u, password);
				u = out;
				for (size_t i = 1; i < num_iterations; i++) {
					u = hmac.run(u, password);
					for (size_t i = 0; i < out.size(); i++)
					{
						out[i] ^= u[i];
					}
				}

				return out;
			};

			std::string output;
			uint32_t counter = 1;
			do {
				output += F(counter);
				counter++;
			} while (output.size() < num_bytes);
			return output.substr(0, num_bytes);
		}
	}
}