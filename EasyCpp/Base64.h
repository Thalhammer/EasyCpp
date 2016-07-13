#pragma once
#include "DllExport.h"
#include <string>
#include <vector>
#include <cstdint>

namespace EasyCpp
{

	/// <summary>Base64 class for converting binary data to strings and back</summary>
	class DLL_EXPORT Base64
	{
	private:
		Base64();
		virtual ~Base64();
		static void single(std::string& res, uint8_t d1, uint8_t d2, uint8_t d3);
		static void single(std::vector<uint8_t>& res, const char* ptr);
	public:
		/// <summary>Converts a std::string to a base64 encoded string.</summary>
		/// <param>The string to convert</param>
		/// <returns>Base64 encoded string</returns>
		static std::string toString(const std::string& str);
		/// <summary>Converts a data array to a base64 encoded string.</summary>
		/// <param>The data to convert</param>
		/// <returns>Base64 encoded string</returns>
		static std::string toString(const std::vector<uint8_t>& data);
		/// <summary>Converts a base64 encoded string to binary data.</summary>
		/// <param>The string to convert</param>
		/// <returns>The contained binary data</returns>
		/// <exception cref="std::runtime_error">Thrown if the passed base64 string is invalid</exception>
		static std::vector<uint8_t> toBinary(const std::string& str);
	};

}
