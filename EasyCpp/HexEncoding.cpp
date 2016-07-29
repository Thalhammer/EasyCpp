#include "HexEncoding.h"
#include <sstream>
#include <iomanip>

std::string EasyCpp::HexEncoding::encode(const std::string & str)
{
	std::string res;
	res.resize(str.size() * 2);
	for (size_t i = 0; i < str.length(); i++)
	{
		res[i * 2] = (((uint8_t)str[i] >> 4) > 0x09) ? ('a' + (((uint8_t)str[i] >> 4) - 0x0a)) : ('0' + ((uint8_t)str[i] >> 4));
		res[(i * 2) + 1] = (((uint8_t)str[i] & 0x0f) > 0x09) ? ('a' + (((uint8_t)str[i] & 0x0f)) - 0x0a) : ('0' + ((uint8_t)str[i] & 0x0f));
	}
	return res;
}

std::string EasyCpp::HexEncoding::decode(const std::string & hex)
{
	if (hex.size() % 2 != 0) throw std::runtime_error("Invalid hex string");
	std::string res;
	res.resize(hex.size() / 2);
	for (size_t i = 0; i < hex.size(); i += 2)
	{
		unsigned char c = 0x00;
		if (hex[i] >= '0'&&hex[i] <= '9') {
			c = (hex[i] - '0') << 4;
		}
		else if (hex[i] >= 'A' && hex[i] <= 'F') {
			c = (hex[i] - 'A' + 0x0a) << 4;
		}
		else if (hex[i] >= 'a' && hex[i] <= 'f') {
			c = (hex[i] - 'a' + 0x0a) << 4;
		}

		if (hex[i + 1] >= '0'&&hex[i + 1] <= '9') {
			c |= hex[i + 1] - '0';
		}
		else if (hex[i + 1] >= 'A' && hex[i + 1] <= 'F') {
			c |= hex[i + 1] - 'A' + 0x0a;
		}
		else if (hex[i + 1] >= 'a' && hex[i + 1] <= 'f') {
			c |= hex[i + 1] - 'a' + 0x0a;
		}
		res[i / 2] = c;
	}
	return res;
}
