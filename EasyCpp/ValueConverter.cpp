#include "ValueConverter.h"
#include "ConvertException.h"
#include <string>
#ifdef _WIN32
#include <locale>
#include <codecvt>
#elif defined(__linux__)
#include <cwchar>
#endif

namespace EasyCpp
{
	std::map<size_t, std::map<size_t, ValueConverter::converter_function_t>> ValueConverter::_converters;
	bool ValueConverter::_init_done = false;
	std::recursive_mutex ValueConverter::_mtx;

	void ValueConverter::convert(const std::type_info& from_type, void* from_val, const std::type_info& to_type, void* to_val)
	{
		std::unique_lock<std::recursive_mutex> lck(_mtx);
		Init();
		if (isConvertable(from_type, to_type)) {
			auto fn = _converters[from_type.hash_code()][to_type.hash_code()];
			lck.unlock();
			fn(from_val, to_val);
		}
		else
			throw std::runtime_error("No converter found");
	}

	bool ValueConverter::isConvertable(const std::type_info& from, const std::type_info& to)
	{
		std::lock_guard<std::recursive_mutex> lck(_mtx);
		Init();
		return _converters.count(from.hash_code()) != 0 && _converters[from.hash_code()].count(to.hash_code()) != 0;
	}

	void ValueConverter::setConverter(const std::type_info& from, const std::type_info& to, converter_function_t fn)
	{
		std::lock_guard<std::recursive_mutex> lck(_mtx);
		Init();
		_converters[from.hash_code()][to.hash_code()] = fn;
	}

	// cppcheck-suppress variableScope
	void ValueConverter::Init()
	{
		std::lock_guard<std::recursive_mutex> lck(_mtx);
		if (_init_done) return;
		_init_done = true;
		// Bool
		CONVERT_FN(bool, std::string, {
			to = (from ? "true" : "false");
		});
		CONVERT_FN(bool, std::wstring, {
			to = (from ? L"true" : L"false");
		});
		CONVERT_FN(bool, int8_t, {
			to = from ? 1 : 0;
		});
		CONVERT_FN(bool, int16_t, {
			to = from ? 1 : 0;
		});
		CONVERT_FN(bool, int32_t, {
			to = from ? 1 : 0;
		});
		CONVERT_FN(bool, int64_t, {
			to = from ? 1 : 0;
		});
		CONVERT_FN(bool, uint8_t, {
			to = from ? 1 : 0;
		});
		CONVERT_FN(bool, uint16_t, {
			to = from ? 1 : 0;
		});
		CONVERT_FN(bool, uint32_t, {
			to = from ? 1 : 0;
		});
		CONVERT_FN(bool, uint64_t, {
			to = from ? 1 : 0;
		});
		// String
#if defined(_WIN32)
		CONVERT_FN(std::string, std::wstring, {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		to = conv.from_bytes(from);
		});
#elif defined(__linux__)
		CONVERT_FN(std::string, std::wstring, {
			std::mbstate_t state = std::mbstate_t();
		const char* mbstr = from.data();
		int len = 1 + std::mbsrtowcs(NULL, &mbstr, 0, &state);
		to.resize(len);
		std::mbsrtowcs(&to[0], &mbstr, to.size(), &state);
		to.pop_back();
		});
#endif
		CONVERT_FN(std::string, const char*, {
			to = from.c_str();
		});
		CONVERT_FN(std::string, bool, {
			if (from == "true")
			to = true;
			else if (from == "false")
				to = false;
			else throw ConvertException("Could not convert string to bool: \"" + from + "\" is not a valid bool value");
		});
		CONVERT_FN(std::string, int8_t, {
			long long val = std::stoll(from);
		if (val > INT8_MAX || val < INT8_MIN)
			throw ConvertException("Could not convert string to int8: Value exceeds integer range");
		to = (int8_t)val;
		});
		CONVERT_FN(std::string, int16_t, {
			long long val = std::stoll(from);
		if (val > INT16_MAX || val < INT16_MIN)
			throw ConvertException("Could not convert string to int16: Value exceeds integer range");
		to = (int16_t)val;
		});
		CONVERT_FN(std::string, int32_t, {
			long long val = std::stoll(from);
		if (val > INT32_MAX || val < INT32_MIN)
			throw ConvertException("Could not convert string to int32: Value exceeds integer range");
		to = (int32_t)val;
		});
		CONVERT_FN(std::string, int64_t, {
			to = std::stoll(from);
		});
		CONVERT_FN(std::string, uint8_t, {
			unsigned long long val = std::stoull(from);
		if (val > UINT8_MAX)
			throw ConvertException("Could not convert string to uint8: Value exceeds integer range");
		to = (uint8_t)val;
		});
		CONVERT_FN(std::string, uint16_t, {
			unsigned long long val = std::stoull(from);
		if (val > UINT16_MAX)
			throw ConvertException("Could not convert string to uint16: Value exceeds integer range");
		to = (uint16_t)val;
		});
		CONVERT_FN(std::string, uint32_t, {
			unsigned long long val = std::stoull(from);
		if (val > UINT32_MAX)
			throw ConvertException("Could not convert string to uint32: Value exceeds integer range");
		to = (uint32_t)val;
		});
		CONVERT_FN(std::string, uint64_t, {
			to = std::stoull(from);
		});
		CONVERT_FN(std::string, float, {
			to = std::stof(from);
		});
		CONVERT_FN(std::string, double, {
			to = std::stod(from);
		});
		CONVERT_FN(std::string, long double, {
			to = std::stold(from);
		});
		// WString
#if defined(_WIN32)
		CONVERT_FN(std::wstring, std::string, {
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> conv;
		to = conv.to_bytes(from);
		});
#elif defined(__linux__)
		CONVERT_FN(std::wstring, std::string, {
			std::mbstate_t state = {};
		const wchar_t* p = from.data();
		int len = std::wcsrtombs(NULL, &p, 0, &state) + 1;
		to.resize(len);
		std::wcsrtombs(&to[0], &p, to.size(), &state);
		to.pop_back();
		});
#endif
		CONVERT_FN(std::wstring, bool, {
			if (from == L"true")
			to = true;
			else if (from == L"false")
				to = false;
			else throw ConvertException("Could not convert wstring to bool: \"" + ValueConverter::convert<std::wstring,std::string>(from) + "\" is not a valid bool value");
		});
		CONVERT_FN(std::wstring, int8_t, {
			long long val = std::stoll(from);
		if (val > INT8_MAX || val < INT8_MIN)
			throw ConvertException("Could not convert string to int8: Value exceeds integer range");
		to = (int8_t)val;
		});
		CONVERT_FN(std::wstring, int16_t, {
			long long val = std::stoll(from);
		if (val > INT16_MAX || val < INT16_MIN)
			throw ConvertException("Could not convert string to int16: Value exceeds integer range");
		to = (int16_t)val;
		});
		CONVERT_FN(std::wstring, int32_t, {
			long long val = std::stoll(from);
		if (val > INT32_MAX || val < INT32_MIN)
			throw ConvertException("Could not convert string to int32: Value exceeds integer range");
		to = (int32_t)val;
		});
		CONVERT_FN(std::wstring, int64_t, {
			to = std::stoll(from);
		});
		CONVERT_FN(std::wstring, uint8_t, {
			unsigned long long val = std::stoull(from);
		if (val > UINT8_MAX)
			throw ConvertException("Could not convert string to uint8: Value exceeds integer range");
		to = (uint8_t)val;
		});
		CONVERT_FN(std::wstring, uint16_t, {
			unsigned long long val = std::stoull(from);
		if (val > UINT16_MAX)
			throw ConvertException("Could not convert string to uint16: Value exceeds integer range");
		to = (uint16_t)val;
		});
		CONVERT_FN(std::wstring, uint32_t, {
			unsigned long long val = std::stoull(from);
		if (val > UINT32_MAX)
			throw ConvertException("Could not convert string to uint32: Value exceeds integer range");
		to = (uint32_t)val;
		});
		CONVERT_FN(std::wstring, uint64_t, {
			to = std::stoull(from);
		});
		CONVERT_FN(std::wstring, float, {
			to = std::stof(from);
		});
		CONVERT_FN(std::wstring, double, {
			to = std::stod(from);
		});
		CONVERT_FN(std::wstring, long double, {
			to = std::stold(from);
		});

		// Uint8
		CONVERT_FN(uint8_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(uint8_t, std::string, {
			to = std::to_string((int)from);
		});
		CONVERT_FN(uint8_t, std::wstring, {
			to = std::to_wstring((int)from);
		});
		CONVERT_FN(uint8_t, int8_t, {
			if (from > INT8_MAX)
			throw ConvertException("Could not convert uint8 to int8: Value exceeds integer range");
		to = from;
		});
		CONVERT_FN(uint8_t, int16_t, {
			to = from;
		});
		CONVERT_FN(uint8_t, int32_t, {
			to = from;
		});
		CONVERT_FN(uint8_t, int64_t, {
			to = from;
		});
		CONVERT_FN(uint8_t, uint8_t, {
			to = from;
		});
		CONVERT_FN(uint8_t, uint16_t, {
			to = from;
		});
		CONVERT_FN(uint8_t, uint32_t, {
			to = from;
		});
		CONVERT_FN(uint8_t, uint64_t, {
			to = from;
		});
		CONVERT_FN(uint8_t, float, {
			to = from;
		});
		CONVERT_FN(uint8_t, double, {
			to = from;
		});
		CONVERT_FN(uint8_t, long double, {
			to = from;
		});

		// Uint16
		CONVERT_FN(uint16_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(uint16_t, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(uint16_t, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(uint16_t, int8_t, {
			if (from > INT8_MAX)
			throw ConvertException("Could not convert uint16 to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(uint16_t, int16_t, {
			if (from > INT16_MAX)
			throw ConvertException("Could not convert uint16 to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(uint16_t, int32_t, {
			to = from;
		});
		CONVERT_FN(uint16_t, int64_t, {
			to = from;
		});
		CONVERT_FN(uint16_t, uint8_t, {
			if (from > UINT8_MAX)
			throw ConvertException("Could not convert uint16 to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(uint16_t, uint16_t, {
			to = from;
		});
		CONVERT_FN(uint16_t, uint32_t, {
			to = from;
		});
		CONVERT_FN(uint16_t, uint64_t, {
			to = from;
		});
		CONVERT_FN(uint16_t, float, {
			to = from;
		});
		CONVERT_FN(uint16_t, double, {
			to = from;
		});
		CONVERT_FN(uint16_t, long double, {
			to = from;
		});

		// Uint32
		CONVERT_FN(uint32_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(uint32_t, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(uint32_t, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(uint32_t, int8_t, {
			if (from > INT8_MAX)
			throw ConvertException("Could not convert uint32 to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(uint32_t, int16_t, {
			if (from > INT16_MAX)
			throw ConvertException("Could not convert uint32 to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(uint32_t, int32_t, {
			if (from > INT32_MAX)
			throw ConvertException("Could not convert uint32 to int32: Value exceeds integer range");
		to = from;
		});
		CONVERT_FN(uint32_t, int64_t, {
			to = from;
		});
		CONVERT_FN(uint32_t, uint8_t, {
			if (from > UINT8_MAX)
			throw ConvertException("Could not convert uint32 to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(uint32_t, uint16_t, {
			if (from > UINT16_MAX)
			throw ConvertException("Could not convert uint32 to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(uint32_t, uint32_t, {
			to = from;
		});
		CONVERT_FN(uint32_t, uint64_t, {
			to = from;
		});
		CONVERT_FN(uint32_t, float, {
			to = (float)from;
		});
		CONVERT_FN(uint32_t, double, {
			to = from;
		});
		CONVERT_FN(uint32_t, long double, {
			to = from;
		});

		// Uint64
		CONVERT_FN(uint64_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(uint64_t, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(uint64_t, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(uint64_t, int8_t, {
			if (from > INT8_MAX)
			throw ConvertException("Could not convert uint64 to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(uint64_t, int16_t, {
			if (from > INT16_MAX)
			throw ConvertException("Could not convert uint64 to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(uint64_t, int32_t, {
			if (from > INT32_MAX)
			throw ConvertException("Could not convert uint64 to int32: Value exceeds integer range");
		to = (int32_t)from;
		});
		CONVERT_FN(uint64_t, int64_t, {
			if (from > INT64_MAX)
			throw ConvertException("Could not convert uint64 to int64: Value exceeds integer range");
		to = from;
		});
		CONVERT_FN(uint64_t, uint8_t, {
			if (from > UINT8_MAX)
			throw ConvertException("Could not convert uint64 to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(uint64_t, uint16_t, {
			if (from > UINT16_MAX)
			throw ConvertException("Could not convert uint64 to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(uint64_t, uint32_t, {
			if (from > UINT32_MAX)
			throw ConvertException("Could not convert uint64 to uint32: Value exceeds integer range");
		to = (uint32_t)from;
		});
		CONVERT_FN(uint64_t, uint64_t, {
			to = from;
		});
		CONVERT_FN(uint64_t, float, {
			to = (float)from;
		});
		CONVERT_FN(uint64_t, double, {
			to = (double)from;
		});
		CONVERT_FN(uint64_t, long double, {
			to = (long double)from;
		});

		// Int8
		CONVERT_FN(int8_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(int8_t, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(int8_t, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(int8_t, int8_t, {
			to = from;
		});
		CONVERT_FN(int8_t, int16_t, {
			to = from;
		});
		CONVERT_FN(int8_t, int32_t, {
			to = from;
		});
		CONVERT_FN(int8_t, int64_t, {
			to = from;
		});
		CONVERT_FN(int8_t, uint8_t, {
			if (from < 0)
			throw ConvertException("Could not convert int8 to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(int8_t, uint16_t, {
			if (from < 0)
			throw ConvertException("Could not convert int8 to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(int8_t, uint32_t, {
			if (from < 0)
			throw ConvertException("Could not convert int8 to uint32: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(int8_t, uint64_t, {
			if (from < 0)
			throw ConvertException("Could not convert int8 to uint64: Value exceeds integer range");
		to = (uint64_t)from;
		});
		CONVERT_FN(int8_t, float, {
			to = from;
		});
		CONVERT_FN(int8_t, double, {
			to = from;
		});
		CONVERT_FN(int8_t, long double, {
			to = from;
		});

		// Int16
		CONVERT_FN(int16_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(int16_t, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(int16_t, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(int16_t, int8_t, {
			if (from > INT8_MAX || from < INT8_MIN)
			throw ConvertException("Could not convert int16 to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(int16_t, int16_t, {
			to = from;
		});
		CONVERT_FN(int16_t, int32_t, {
			to = from;
		});
		CONVERT_FN(int16_t, int64_t, {
			to = from;
		});
		CONVERT_FN(int16_t, uint8_t, {
			if (from < 0 || from > UINT8_MAX)
			throw ConvertException("Could not convert int16 to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(int16_t, uint16_t, {
			if (from < 0)
			throw ConvertException("Could not convert int16 to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(int16_t, uint32_t, {
			if (from < 0)
			throw ConvertException("Could not convert int16 to uint32: Value exceeds integer range");
		to = (uint32_t)from;
		});
		CONVERT_FN(int16_t, uint64_t, {
			if (from < 0)
			throw ConvertException("Could not convert int16 to uint64: Value exceeds integer range");
		to = (uint64_t)from;
		});
		CONVERT_FN(int16_t, float, {
			to = from;
		});
		CONVERT_FN(int16_t, double, {
			to = from;
		});
		CONVERT_FN(int16_t, long double, {
			to = from;
		});

		// Int32
		CONVERT_FN(int32_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(int32_t, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(int32_t, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(int32_t, int8_t, {
			if (from > INT8_MAX || from < INT8_MIN)
			throw ConvertException("Could not convert int32 to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(int32_t, int16_t, {
			if (from > INT16_MAX || from < INT16_MIN)
			throw ConvertException("Could not convert int32 to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(int32_t, int32_t, {
			to = from;
		});
		CONVERT_FN(int32_t, int64_t, {
			to = from;
		});
		CONVERT_FN(int32_t, uint8_t, {
			if (from < 0 || from > UINT8_MAX)
			throw ConvertException("Could not convert int32 to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(int32_t, uint16_t, {
			if (from < 0 || from > UINT16_MAX)
			throw ConvertException("Could not convert int32 to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(int32_t, uint32_t, {
			if (from < 0)
			throw ConvertException("Could not convert int32 to uint32: Value exceeds integer range");
		to = (uint32_t)from;
		});
		CONVERT_FN(int32_t, uint64_t, {
			if (from < 0)
			throw ConvertException("Could not convert int32 to uint64: Value exceeds integer range");
		to = (uint64_t)from;
		});
		CONVERT_FN(int32_t, float, {
			to = (float)from;
		});
		CONVERT_FN(int32_t, double, {
			to = from;
		});
		CONVERT_FN(int32_t, long double, {
			to = from;
		});

		// Int64
		CONVERT_FN(int64_t, bool, {
			to = from == 1;
		});
		CONVERT_FN(int64_t, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(int64_t, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(int64_t, int8_t, {
			if (from > INT8_MAX || from < INT8_MIN)
			throw ConvertException("Could not convert int64 to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(int64_t, int16_t, {
			if (from > INT16_MAX || from < INT16_MIN)
			throw ConvertException("Could not convert int64 to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(int64_t, int32_t, {
			if (from > INT32_MAX || from < INT32_MIN)
			throw ConvertException("Could not convert int64 to int32: Value exceeds integer range");
		to = (int32_t)from;
		});
		CONVERT_FN(int64_t, int64_t, {
			to = from;
		});
		CONVERT_FN(int64_t, uint8_t, {
			if (from < 0 || from > UINT8_MAX)
			throw ConvertException("Could not convert int64 to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(int64_t, uint16_t, {
			if (from < 0 || from > UINT16_MAX)
			throw ConvertException("Could not convert int64 to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(int64_t, uint32_t, {
			if (from < 0 || from > UINT32_MAX)
			throw ConvertException("Could not convert int64 to uint32: Value exceeds integer range");
		to = (uint32_t)from;
		});
		CONVERT_FN(int64_t, uint64_t, {
			if (from < 0)
			throw ConvertException("Could not convert int64 to uint64: Value exceeds integer range");
		to = (uint64_t)from;
		});
		CONVERT_FN(int64_t, float, {
			to = (float)from;
		});
		CONVERT_FN(int64_t, double, {
			to = (double)from;
		});
		CONVERT_FN(int64_t, long double, {
			to = (long double)from;
		});

		// Float
		CONVERT_FN(float, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(float, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(float, int8_t, {
			if (from > INT8_MAX || from < INT8_MIN)
			throw ConvertException("Could not convert float to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(float, int16_t, {
			if (from > INT16_MAX || from < INT16_MIN)
			throw ConvertException("Could not convert float to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(float, int32_t, {
			if (from > INT32_MAX || from < INT32_MIN)
			throw ConvertException("Could not convert float to int32: Value exceeds integer range");
		to = (int32_t)from;
		});
		CONVERT_FN(float, int64_t, {
			if (from > INT64_MAX || from < INT64_MIN)
			throw ConvertException("Could not convert float to int64: Value exceeds integer range");
		to = (int64_t)from;
		});
		CONVERT_FN(float, uint8_t, {
			if (from > UINT8_MAX || from < 0)
			throw ConvertException("Could not convert float to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(float, uint16_t, {
			if (from > UINT16_MAX || from < 0)
			throw ConvertException("Could not convert float to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(float, uint32_t, {
			if (from > UINT32_MAX || from < 0)
			throw ConvertException("Could not convert float to uint32: Value exceeds integer range");
		to = (uint32_t)from;
		});
		CONVERT_FN(float, uint64_t, {
			if (from > UINT64_MAX || from < 0)
			throw ConvertException("Could not convert float to uint64: Value exceeds integer range");
		to = (uint64_t)from;
		});
		CONVERT_FN(float, float, {
			to = from;
		});
		CONVERT_FN(float, double, {
			to = from;
		});
		CONVERT_FN(float, long double, {
			to = from;
		});

		// Double
		CONVERT_FN(double, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(double, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(double, int8_t, {
			if (from > INT8_MAX || from < INT8_MIN)
			throw ConvertException("Could not convert double to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(double, int16_t, {
			if (from > INT16_MAX || from < INT16_MIN)
			throw ConvertException("Could not convert double to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(double, int32_t, {
			if (from > INT32_MAX || from < INT32_MIN)
			throw ConvertException("Could not convert double to int32: Value exceeds integer range");
		to = (int32_t)from;
		});
		CONVERT_FN(double, int64_t, {
			if (from > INT64_MAX || from < INT64_MIN)
			throw ConvertException("Could not convert double to int64: Value exceeds integer range");
		to = (int64_t)from;
		});
		CONVERT_FN(double, uint8_t, {
			if (from > UINT8_MAX || from < 0)
			throw ConvertException("Could not convert double to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(double, uint16_t, {
			if (from > UINT16_MAX || from < 0)
			throw ConvertException("Could not convert double to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(double, uint32_t, {
			if (from > UINT32_MAX || from < 0)
			throw ConvertException("Could not convert double to uint32: Value exceeds integer range");
		to = (uint32_t)from;
		});
		CONVERT_FN(double, uint64_t, {
			if (from > UINT64_MAX || from < 0)
			throw ConvertException("Could not convert double to uint64: Value exceeds integer range");
		to = (uint64_t)from;
		});
		CONVERT_FN(double, float, {
			to = (float)from;
		});
		CONVERT_FN(double, double, {
			to = from;
		});
		CONVERT_FN(double, long double, {
			to = from;
		});

		// Long Double
		CONVERT_FN(long double, std::string, {
			to = std::to_string(from);
		});
		CONVERT_FN(long double, std::wstring, {
			to = std::to_wstring(from);
		});
		CONVERT_FN(long double, int8_t, {
			if (from > INT8_MAX || from < INT8_MIN)
			throw ConvertException("Could not convert long double to int8: Value exceeds integer range");
		to = (int8_t)from;
		});
		CONVERT_FN(long double, int16_t, {
			if (from > INT16_MAX || from < INT16_MIN)
			throw ConvertException("Could not convert long double to int16: Value exceeds integer range");
		to = (int16_t)from;
		});
		CONVERT_FN(long double, int32_t, {
			if (from > INT32_MAX || from < INT32_MIN)
			throw ConvertException("Could not convert long double to int32: Value exceeds integer range");
		to = (int32_t)from;
		});
		CONVERT_FN(long double, int64_t, {
			if (from > INT64_MAX || from < INT64_MIN)
			throw ConvertException("Could not convert long double to int64: Value exceeds integer range");
		to = (int64_t)from;
		});
		CONVERT_FN(long double, uint8_t, {
			if (from > UINT8_MAX || from < 0)
			throw ConvertException("Could not convert long double to uint8: Value exceeds integer range");
		to = (uint8_t)from;
		});
		CONVERT_FN(long double, uint16_t, {
			if (from > UINT16_MAX || from < 0)
			throw ConvertException("Could not convert long double to uint16: Value exceeds integer range");
		to = (uint16_t)from;
		});
		CONVERT_FN(long double, uint32_t, {
			if (from > UINT32_MAX || from < 0)
			throw ConvertException("Could not convert long double to uint32: Value exceeds integer range");
		to = (uint32_t)from;
		});
		CONVERT_FN(long double, uint64_t, {
			if (from > UINT64_MAX || from < 0)
			throw ConvertException("Could not convert long double to uint64: Value exceeds integer range");
		to = (uint64_t)from;
		});
		CONVERT_FN(long double, float, {
			to = (float)from;
		});
		CONVERT_FN(long double, double, {
			to = (double)from;
		});
		CONVERT_FN(long double, long double, {
			to = from;
		});

		// Const string
		CONVERT_FN(const char*, std::string, {
			to = from;
		});
		CONVERT_FN(std::vector<uint8_t>, std::string, {
			to = std::string(from.begin(),from.end());
		});
		CONVERT_FN(std::string, std::vector<uint8_t>, {
			to = std::vector<uint8_t>(from.begin(),from.end());
		});
	}
}
