#include "BinaryReader.h"

namespace EasyCpp
{
	namespace VFS
	{
		BinaryReader::BinaryReader(InputStreamPtr stream)
			:_stream(stream)
		{
		}

		std::vector<uint8_t> BinaryReader::readBytes(size_t len)
		{
			return _stream->read(len);
		}

		InputStreamPtr BinaryReader::getStream()
		{
			return _stream;
		}
	}
}