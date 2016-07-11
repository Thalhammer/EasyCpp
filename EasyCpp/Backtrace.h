#pragma once
#include "DllExport.h"
#include <vector>
#include <string>

namespace EasyCpp
{
	//! \ingroup Util
	//! \class Backtrace
	//! \brief Generates a backtrace in the current location.
	//! Requires a Debugbuild to provide useful information.
	class DLL_EXPORT Backtrace
	{
	public:
		//! \brief Konstruktor
		Backtrace();
#ifdef _WIN32
		Backtrace(void* context);
#endif
		~Backtrace();

		//! \brief Print this Backtrace to the given ostream.
		//! \param out The output stream
		void print(std::ostream& out) const;
		//! \brief Returns this backtrace as a human readable string.
		//! \return A std::string representing this backtrace.
		std::string print() const;
		//! \brief Get the number of stackframes captured.
		//! \return Number of frames
		size_t size() const;
		//! \brief Get the frame name at a specific index.
		//! \param pos The index to get.
		//! \return A std::string representing this particular frame.
		std::string at(size_t pos) const;
	private:
		std::vector<std::string> _trace;
#ifdef _WIN32
		void doBacktrace(void* context);
#endif
	};
}
