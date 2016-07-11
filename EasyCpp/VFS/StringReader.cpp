#include "StringReader.h"

namespace EasyCpp
{
	namespace VFS
	{
		StringReader::StringReader(InputStreamPtr stream)
			:_stream(stream)
		{
		}

		std::string StringReader::read(size_t s)
		{
			auto res = _stream->read(s);
			return std::string(res.begin(), res.end());
		}

		std::string StringReader::readToEnd()
		{
			std::string res = "";
			while (_stream->isGood())
			{
				auto data = _stream->read(4096);
				res += std::string(data.begin(), data.end());
			}
			return res;
		}

		std::string StringReader::readLine()
		{
			std::string res;
			while (true)
			{
				auto data = _stream->read(1);
				if (data.size() != 1 || !_stream->isGood() || data[0] == '\n')
					break;
				res += (char)data[0];
			}
			return res;
		}

		InputStreamPtr StringReader::getStream()
		{
			return _stream;
		}

	}
}
