#include "Base64URL.h"
#include "Base64.h"

namespace EasyCpp
{

	std::string Base64URL::toString(const std::string & str)
	{
		std::string b64 = Base64::toString(str);
		for (size_t i = 0; i < b64.size(); i++)
		{
			if (b64[i] == '+') b64[i] = '-';
			else if (b64[i] == '/') b64[i] = '_';
		}
		return b64;
	}

	std::string Base64URL::toString(const std::vector<uint8_t>& data)
	{
		std::string b64 = Base64::toString(data);
		for (size_t i = 0; i < b64.size(); i++)
		{
			if (b64[i] == '+') b64[i] = '-';
			else if (b64[i] == '/') b64[i] = '_';
		}
		return b64;
	}

	std::vector<uint8_t> Base64URL::toBinary(const std::string & str)
	{
		std::string b64 = str;
		for (size_t i = 0; i < b64.size(); i++)
		{
			if (b64[i] == '-') b64[i] = '+';
			else if (b64[i] == '_') b64[i] = '/';
		}
		return Base64::toBinary(b64);
	}

}