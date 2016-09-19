#include "Base64.h"
#include <stdexcept>

namespace EasyCpp
{

	Base64::Base64()
	{
	}


	Base64::~Base64()
	{
	}

	static char getByValue(uint8_t v)
	{
		static char _charTable[64]{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
			'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
			'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
			'w', 'x', 'y', 'z', '0', '1', '2', '3',
			'4', '5', '6', '7', '8', '9', '+', '/'
		};
		if (v > 63)
			throw std::runtime_error("Internal error");
		return _charTable[v];
	}

	static uint8_t getByChar(const char c)
	{
		if (c >= 'A'&&c <= 'Z')
			return c - 'A';
		if (c >= 'a'&&c <= 'z')
			return c - 'a' + 26;
		if (c >= '0' && c <= '9')
			return c - '0' + 52;
		if (c == '+')
			return 62;
		if (c == '/')
			return 63;
		if (c == '=')
			return 0;
		throw std::runtime_error(std::string("Invalid character ") + c);
	}

	std::string Base64::toString(const std::string& str)
	{
		return Base64::toString(std::vector<uint8_t>(str.begin(), str.end()));
	}

	std::string Base64::toString(const std::vector<uint8_t>& data)
	{
		std::string res;
		res.reserve((data.size() * 4) / 3);
		for (size_t i = 0; i < data.size(); i += 3)
		{
			if (data.size() - i < 3)
			{
				if (data.size() - i == 2)
				{
					single(res, data[i], data[i + 1], 0);
					res[res.size() - 1] = '=';
				}
				else {
					single(res, data[i], 0, 0);
					res[res.size() - 1] = '=';
					res[res.size() - 2] = '=';
				}
			}
			else {
				single(res, data[i], data[i + 1], data[i + 2]);
			}
		}
		return res;
	}

	std::vector<uint8_t> Base64::toBinary(const std::string& str)
	{
		std::vector<uint8_t> res;
		if (str.size() % 4 != 0)
			throw std::runtime_error("Invalid base64");
		res.reserve(str.size() * 3 / 4);
		for (size_t i = 0; i < str.size(); i += 4)
		{
			single(res, &str[i]);
		}
		return res;
	}

	void Base64::single(std::string& res, uint8_t d1, uint8_t d2, uint8_t d3)
	{
		res += getByValue(d1 >> 2);
		res += getByValue((d1 << 4 | d2 >> 4) & 0x3f);
		res += getByValue((d2 << 2 | d3 >> 6) & 0x3f);
		res += getByValue(d3 & 0x3f);
	}

	void Base64::single(std::vector<uint8_t>& res, const char* ptr)
	{
		uint8_t d1 = getByChar(ptr[0]);
		uint8_t d2 = getByChar(ptr[1]);
		uint8_t d3 = getByChar(ptr[2]);
		uint8_t d4 = getByChar(ptr[3]);
		res.push_back(d1 << 2 | d2 >> 4);
		if (ptr[2] != '=')
			res.push_back(d2 << 4 | d3 >> 2);
		if (ptr[3] != '=')
			res.push_back(d3 << 6 | d4);
	}

}
