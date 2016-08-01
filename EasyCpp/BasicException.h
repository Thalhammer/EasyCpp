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
	/// <summary>Baseclass for exceptions which contains a Backtrace.</summary>
	class DLL_EXPORT BasicException :
		public std::exception
	{
	public:
		BasicException() noexcept;
		virtual ~BasicException();
		virtual const char* what() const noexcept;

		/// <summary>Get a backtrace to the moment where this exception was instanciated.</summary>
		/// <returns>A reference to a backtrace.</returns>
		const Backtrace& backtrace() const;
	private:
		Backtrace _trace;
	};
}
#ifdef _MSC_VER
#pragma warning( pop )
#endif
