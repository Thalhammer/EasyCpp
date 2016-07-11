#pragma once
#include "DllExport.h"
#include <string>
#include <vector>
#include <cstdint>

namespace EasyCpp
{

	//! \ingroup Util
	//! \class Base64
	//! Base64 Klasse für die Konvertierung von Binärdaten in strings und zurück.
	class DLL_EXPORT Base64
	{
	private:
		Base64();
		virtual ~Base64();
		static void single(std::string& res, uint8_t d1, uint8_t d2, uint8_t d3);
		static void single(std::vector<uint8_t>& res, const char* ptr);
	public:
		//! Konvertiert einen std::string in einen Base64 encodierten String.
		//! Dies ermöglicht das fehlerfreie übertragen von UTF-8 über 7 bit Medien.
		//! @param str Der zu verarbeitende string.
		//! @return Die Base64 Repräsentation der Eingabedaten.
		static std::string toString(const std::string& str);
		//! Konvertiert Binärdaten in einen Base64 String.
		//! @param data Die zu verarbeitenden Daten.
		//! @return Die Base64 Repräsentation der Eingabedaten.
		static std::string toString(const std::vector<uint8_t>& data);
		//! Konvertiert einen Base64 String in Binärdaten.
		//! @param str Der Base64 encodierte String.
		//! @return Die decodierten Binärdaten.
		//! @throws std::runtime_error Fehlerhafter eingabe string.
		static std::vector<uint8_t> toBinary(const std::string& str);
	};

}
