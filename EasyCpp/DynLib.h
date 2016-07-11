#pragma once
#include "DllExport.h"

#include <string>
#include <functional>
#include <memory>

//! \addtogroup Util
//! @{

namespace EasyCpp
{

	//! Plattformunabhägige Wrapperklasse für das Laden und verwenden von Dynamischen Bibliotheken.
	class DLL_EXPORT DynLib
	{
	private:
		std::shared_ptr<void> _handle;
		void throwError();
	public:
		//! Konstruktor. Lädt die angegebene Bibliothek.
		//! @throws std::runtime_error Fehler beim laden der Bibliothek.
		DynLib(const std::string& file);
		//! Destruktor
		virtual ~DynLib();
		//! Gibt einen Pointer auf die Funktion zurück.
		//! @param fn Name der gesuchten Funktion.
		//! @return Pointer zur Funktion.
		//! @throws std::runtime_error Fehler beim suchen der Funktion.
		void* getFunction(const std::string& fn);
		//! Gibt ein std::function Object für die gewünschte Funktion zurück.
		//! @param fn Name der gesuchten Funktion.
		//! @return std::function Object für die Funktion.
		//! @throws std::runtime_error Fehler beim suchen der Funktion.
		template<typename T>
		std::function<T> getFunction(const std::string& fn)
		{
			return std::function<T>((T*)getFunction(fn));
		}
	};

}

//! @}
