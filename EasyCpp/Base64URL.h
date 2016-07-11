#pragma once
#include "dllexport.h"
#include <string>
#include <vector>
#include <cstdint>

namespace EasyCpp
{
	//! \ingroup Util
	//! \class Base64URL
	//! Base64URL Klasse für die Konvertierung von Binärdaten in URL kompatible strings und zurück.
	class DLL_EXPORT Base64URL
	{
	private:
		Base64URL() {}
		virtual ~Base64URL() {}
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
