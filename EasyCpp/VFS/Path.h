#pragma once
#include "../DllExport.h"
#include <string>

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT Path
		{
		public:
			Path();
			Path(const std::string& path);
			Path(const std::string& dirname, const std::string& basename);
			Path(const std::string& dirname, const std::string& filename, const std::string& extension);
			virtual ~Path();

			std::string getDirName() const;
			std::string getBaseName() const;
			std::string getExtension() const;
			std::string getFileName() const;

			std::string getString() const;

			bool hasFile() const;
		private:
			void normalize();

			std::string _dirname;
			std::string _filename;
			std::string _extension;
		};

		const Path DLL_EXPORT operator+(Path const& lhs, Path const& rhs);
	}
}