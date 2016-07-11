#pragma once
#include "dllexport.h"
#include <string>
#include <vector>
#include <cstdint>

namespace EasyCpp
{
	//! \ingroup Util
	//! \class Base64URL
	//! Base64URL Klasse f�r die Konvertierung von Bin�rdaten in URL kompatible strings und zur�ck.
	class DLL_EXPORT Base64URL
	{
	private:
		Base64URL() {}
		virtual ~Base64URL() {}
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
