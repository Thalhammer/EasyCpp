#pragma once
#include "DllExport.h"
#include <vector>
#include <string>

namespace EasyCpp
{
	/// <summary>Generates a backtrace in the current location.</summary>
	/// Requires a Debugbuild to provide useful information.
	class DLL_EXPORT Backtrace
	{
	public:
		Backtrace();
#ifdef _WIN32
		explicit Backtrace(void* context);
#endif
		~Backtrace();

		/// <summary>Print this Backtrace to the given ostream.</summary>
		/// <param name='out'>The output stream</param>
		void print(std::ostream& out) const;
		/// <summary>Returns this backtrace as a human readable string.</summary>
		/// <returns>A std::string representing this backtrace.</returns>
		std::string print() const;
		/// <summary>Get the number of stackframes captured.</summary>
		/// <returns>Number of frames</returns>
		size_t size() const;
		/// <summary>Get the frame name at a specific index.</summary>
		/// <param name='pos'>The index to get.</param>
		/// <returns>A std::string representing this particular frame.</returns>
		std::string at(size_t pos) const;
	private:
		std::vector<std::string> _trace;
#ifdef _WIN32
		void doBacktrace(void* context);
#endif
	};
}
