#include "Path.h"
#include "../StringAlgorithm.h"
#include <regex>

namespace EasyCpp
{
	namespace VFS
	{
		Path::Path()
			: Path("/")
		{}

		Path::Path(const std::string & path)
		{
			auto vector = stringSplit<std::string>("/", path);
			if (path.length() == 0 || path[0] != '/')
				throw std::runtime_error("Invalid path");
			if (vector[vector.size() - 1] != "") {
				std::string filename = vector[vector.size() - 1];
				size_t extpos = filename.find_last_of('.');
				if (extpos != std::string::npos)
				{
					_filename = filename.substr(0, extpos);
					_extension = filename.substr(extpos + 1);
				}
				else _filename = filename;
				vector[vector.size() - 1] = "";
			}
			_dirname = implode<std::string>("/", vector);
			normalize();
		}

		Path::Path(const std::string & dirname, const std::string & basename)
		{
			std::regex fregex("^([^\\/]*)?$");
			if (!std::regex_match(basename, fregex))
				throw std::runtime_error("Not a valid filename");
			if (dirname.length() == 0 || dirname[0] != '/' || dirname[dirname.size() - 1] != '/')
				throw std::runtime_error("Invalid path");
			_dirname = dirname;
			std::string filename = basename;
			size_t extpos = filename.find_last_of('.');
			if (extpos != std::string::npos)
			{
				_filename = filename.substr(0, extpos);
				_extension = filename.substr(extpos + 1);
			}
			else _filename = filename;
			normalize();
		}

		Path::Path(const std::string & dirname, const std::string & filename, const std::string & extension)
		{
			std::regex fregex("^([^\\/\\.]*)?$");
			if (dirname.length() == 0 || dirname[0] != '/' || dirname[dirname.size() - 1] != '/')
				throw std::runtime_error("Invalid path");
			if (!std::regex_match(filename, fregex))
				throw std::runtime_error("Not a valid filename");
			if (!std::regex_match(extension, fregex))
				throw std::runtime_error("Not a valid extension");
			_dirname = dirname;
			_filename = filename;
			_extension = extension;
			normalize();
		}

		Path::~Path()
		{
		}

		std::string Path::getDirName() const
		{
			return _dirname;
		}

		std::string Path::getBaseName() const
		{
			std::string res = _filename;
			if (_extension != "")
				res += "." + _extension;
			return res;
		}

		std::string Path::getExtension() const
		{
			return _extension;
		}

		std::string Path::getFileName() const
		{
			return _filename;
		}

		std::string Path::getString() const
		{
			std::string bname = getBaseName();
			return getDirName() + (bname != "" ? bname : "");
		}

		bool Path::hasFile() const
		{
			return _filename != "" || _extension != "";
		}

		void Path::normalize()
		{
			std::vector<std::string> parts = stringSplit("/", _dirname);
			std::vector<std::string> out;
			for (const auto& e : parts)
			{
				if (e == "" || e == ".") {
				}
				else if (e == "..") {
					if (out.size() > 0) {
						out.erase(out.begin() + out.size() - 1);
					}
				}
				else {
					out.push_back(e);
				}
			}
			out.push_back("");
			_dirname = "/" + implode<std::string>("/", out);
		}

		const Path DLL_EXPORT operator+(Path const & lhs, Path const & rhs)
		{
			return Path(lhs.getString() + rhs.getString());
		}
	}
}