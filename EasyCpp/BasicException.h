#pragma once
#include "DllExport.h"
#include <stdexcept>
#include "Backtrace.h"

#ifdef _MSC_VER
#pragma warning( push )
#pragma warning( disable : 4275)
#endif
namespace EasyCpp
{
	//! \ingroup Util
	//! \class BasicException
	//! \brief Basisklasse f�r Exceptions, die eine Backtrace enth�lt.
	class DLL_EXPORT BasicException :
		public std::exception
	{
	public:
		BasicException() noexcept;
		virtual ~BasicException();
		virtual const char* what() const noexcept;

		//! \brief Gibt einen Backtrace zur Funktion zur�ck die diese Exception instanziert hat.
		//! \return Eine Referenz auf eine Instanz der Klasse Backtrace.
		const Backtrace& backtrace() const;
	private:
		Backtrace _trace;
	};
}
#ifdef _MSC_VER
#pragma warning( pop )
#endif
