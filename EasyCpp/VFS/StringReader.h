#pragma once
#include <string>
#include "InputStream.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT StringReader
		{
		public:
			StringReader(InputStreamPtr stream);

			std::string read(size_t s);
			std::string readToEnd();
			std::string readLine();

			InputStreamPtr getStream();
		private:
			InputStreamPtr _stream;
		};
	}
}