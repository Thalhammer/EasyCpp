#pragma once
#include <string>
#include <vector>
#include <locale>
#include <algorithm>

namespace EasyCpp
{
	template<typename T>
	std::vector<T> stringSplit(const T& delim, const T& str)
	{
		std::vector<T> res;
		size_t offset = 0;
		do {
			size_t pos = str.find(delim, offset);
			res.push_back(str.substr(offset, (pos == T::npos) ? pos : (pos - offset)));
			offset = (pos != T::npos) ? pos + delim.length() : pos;
		} while (offset != T::npos);
		return res;
	}

	template<typename T, typename Delim>
	std::vector<T> stringSplit(Delim delim, const T& str)
	{
		return stringSplit(std::string(delim), str);
	}

	inline std::string stringReplace(const std::string& str, const std::string& search, const std::string& replace)
	{
		std::string res = str;
		size_t pos = res.find(search);
		while (pos != std::string::npos)
		{
			res = res.replace(pos, search.length(), replace);
			pos = res.find(search, pos);
		}
		return res;
	}

	template<typename T>
	T stringToLower(const T& str)
	{
		T res = str;
		std::transform(res.begin(), res.end(), res.begin(), ::tolower);
		return res;
	}

	template<typename T>
	T stringToUpper(const T& str)
	{
		T res = str;
		std::transform(res.begin(), res.end(), res.begin(), ::toupper);
		return res;
	}

	template<typename T>
	T implode(const T& delim, const std::vector<T> vect)
	{
		T res = "";
		for (size_t i = 0; i < vect.size(); i++)
		{
			res += vect[i];
			if (i != vect.size() - 1)
			{
				res += delim;
			}
		}
		return res;
	}

	template<typename T>
	T &ltrim(T &s) {
		auto last = s.find_first_not_of(" \t\f\v\n\r");
		s.erase(0, last);
		return s;
	}

	template<typename T>
	inline T &rtrim(T&s) {
		auto last = s.find_last_not_of(" \t\f\v\n\r");
		if (last != T::npos) s.erase(last + 1);
		return s;
	}

	template<typename T>
	inline T &trim(T &s) {
		return ltrim(rtrim(s));
	}
}