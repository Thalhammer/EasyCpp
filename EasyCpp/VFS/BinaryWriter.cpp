#include "BinaryWriter.h"

namespace EasyCpp
{
	namespace VFS
	{
		BinaryWriter::BinaryWriter(OutputStreamPtr stream)
			:_stream(stream)
		{
		}

		void BinaryWriter::writeBytes(const void * ptr, size_t len)
		{
			_stream->write(std::vector<uint8_t>((uint8_t*)ptr, ((uint8_t*)ptr + len)));
		}

		OutputStreamPtr BinaryWriter::getStream()
		{
			return _stream;
		}
	}
}
