#include "HOTP.h"
#include "HMAC.h"

namespace EasyCpp
{
	namespace Hash
	{
		HOTP::HOTP(const std::string & key)
			:_key(key)
		{
		}

		std::string HOTP::generate(uint64_t counter, size_t num_digits)
		{
			std::string message = std::string(8, 0x00);
			for (int i = 0; i < 8; i++) { message[i] = *(((char*)&counter) + 7 - i); }
			std::string hmac_val = HMAC("sha1").run(message, _key);

			uint8_t offset = hmac_val[19] & 0x0f;
			uint32_t bin_code = (((uint32_t)hmac_val[offset]) & 0xff) << 24 | (((uint32_t)hmac_val[offset + 1])&0xff) << 16 | (((uint32_t)hmac_val[offset + 2]) & 0xff) << 8 | (((uint32_t)hmac_val[offset + 3]) & 0xff);
			bin_code = bin_code & 0x7fffffff;
			uint32_t mod = 10;
			for (size_t i = 0; i < num_digits - 1; i++) mod *= 10;
			uint32_t digits = bin_code % mod;
			std::string res = std::to_string(digits);
			return std::string(num_digits - res.size(), '0') + res;
		}
	}
}