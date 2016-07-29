#include "Base32.h"
#include <stdexcept>

namespace EasyCpp
{

	Base32::Base32()
	{
	}


	Base32::~Base32()
	{
	}

	static char getByValue(uint8_t v)
	{
		static char _charTable[32]{
			'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
			'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
			'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
			'Y', 'Z', '2', '3', '4', '5', '6', '7'
		};
		if (v > 31)
			throw std::runtime_error("Internal error");
		return _charTable[v];
	}

	static uint8_t getByChar(const char c)
	{
		if (c >= 'A'&&c <= 'Z')
			return c - 'A';
		if (c >= '2' && c <= '7')
			return c - '2' + 26;
		if (c == '=') return 0;
		throw std::runtime_error("Invalid character " + c);
	}

	std::string Base32::toString(const std::string& str)
	{
		return Base32::toString(std::vector<uint8_t>(str.begin(), str.end()));
	}

	std::string Base32::toString(const std::vector<uint8_t>& data)
	{
		std::string res;
		res.reserve((data.size() * 8) / 5);
		for (size_t i = 0; i < data.size(); i += 5)
		{
			if (data.size() - i < 5)
			{
				switch (data.size() - i) {
				case 1: single(res, data[i], 0, 0, 0, 0); break;
				case 2: single(res, data[i], data[i + 1], 0, 0, 0); break;
				case 3: single(res, data[i], data[i + 1], data[i + 2], 0, 0); break;
				case 4: single(res, data[i], data[i + 1], data[i + 2], data[i + 3], 0); break;
				}
				res += std::string(4 - (data.size() - i), '=');
			}
			else {
				single(res, data[i], data[i + 1], data[i + 2], data[i+3], data[i+4]);
			}
		}
		return res;
	}

	std::vector<uint8_t> Base32::toBinary(const std::string& str)
	{
		std::vector<uint8_t> res;
		if (str.size() % 8 != 0)
			throw std::runtime_error("Invalid base32");
		res.reserve(str.size() * 5 / 8);
		for (size_t i = 0; i < str.size(); i += 8)
		{
			single(res, &str[i]);
		}
		return res;
	}

	void Base32::single(std::string& res, uint8_t d1, uint8_t d2, uint8_t d3, uint8_t d4, uint8_t d5)
	{
		res += getByValue(d1 >> 3);
		res += getByValue((d1 << 2 | d2 >> 6) & 0x1f);
		res += getByValue((d2 >> 1) & 0x1f);
		res += getByValue((d2 << 4 | d3 >> 4) & 0x1f);
		res += getByValue((d3 << 1 | d4 >> 7) & 0x1f);
		res += getByValue((d4 >> 2) & 0x1f);
		res += getByValue((d4 << 3 | d5 >> 5) & 0x1f);
		res += getByValue((d5) & 0x1f);
	}

	void Base32::single(std::vector<uint8_t>& res, const char* ptr)
	{
		uint8_t d1 = getByChar(ptr[0]);
		uint8_t d2 = getByChar(ptr[1]);
		uint8_t d3 = getByChar(ptr[2]);
		uint8_t d4 = getByChar(ptr[3]);
		uint8_t d5 = getByChar(ptr[4]);
		uint8_t d6 = getByChar(ptr[5]);
		uint8_t d7 = getByChar(ptr[6]);
		uint8_t d8 = getByChar(ptr[7]);

		res.push_back(d1 << 3 | d2 >> 2);
		if (ptr[3] != '=')
			res.push_back(d2 << 6 | d3 << 1 | d4 >> 4);
		if (ptr[4] != '=')
			res.push_back(d4 << 4 | d5 >> 1);
		if (ptr[6] != '=')
			res.push_back(d5 << 7 | d6 << 2 | d7 >> 3);
		if (ptr[7] != '=')
			res.push_back(d7 << 5 | d8);
	}

}