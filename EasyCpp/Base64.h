#pragma once
#include "DllExport.h"
#include <string>
#include <vector>
#include <cstdint>

namespace EasyCpp
{

	//! \ingroup Util
	//! \class Base64
	//! Base64 Klasse f�r die Konvertierung von Bin�rdaten in strings und zur�ck.
	class DLL_EXPORT Base64
	{
	private:
		Base64();
		virtual ~Base64();
		static void single(std::string& res, uint8_t d1, uint8_t d2, uint8_t d3);
		static void single(std::vector<uint8_t>& res, const char* ptr);
	public:
		//! Konvertiert einen std::string in einen Base64 encodierten String.
		//! Dies erm�glicht das fehlerfreie �bertragen von UTF-8 �ber 7 bit Medien.
		//! @param str Der zu verarbeitende string.
		//! @return Die Base64 Repr�sentation der Eingabedaten.
		static std::string toString(const std::string& str);
		//! Konvertiert Bin�rdaten in einen Base64 String.
		//! @param data Die zu verarbeitenden Daten.
		//! @return Die Base64 Repr�sentation der Eingabedaten.
		static std::string toString(const std::vector<uint8_t>& data);
		//! Konvertiert einen Base64 String in Bin�rdaten.
		//! @param str Der Base64 encodierte String.
		//! @return Die decodierten Bin�rdaten.
		//! @throws std::runtime_error Fehlerhafter eingabe string.
		static std::vector<uint8_t> toBinary(const std::string& str);
	};

}
