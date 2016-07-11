#pragma once
#include <string>
#include "OutputStream.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT StringWriter
		{
		public:
			StringWriter(OutputStreamPtr stream);

			void write(std::string str);
			OutputStreamPtr getStream();
		private:
			OutputStreamPtr _stream;
		};
	}
}