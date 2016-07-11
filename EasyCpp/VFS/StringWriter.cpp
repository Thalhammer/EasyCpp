#include "StringWriter.h"

namespace EasyCpp
{
	namespace VFS
	{
		StringWriter::StringWriter(OutputStreamPtr stream)
			:_stream(stream)
		{
			if (!_stream)
				throw std::invalid_argument("stream can not be null");
		}

		void StringWriter::write(std::string str)
		{
			std::vector<uint8_t> data(str.begin(), str.end());
			_stream->write(data);
		}

		OutputStreamPtr StringWriter::getStream()
		{
			return _stream;
		}
	}
}

