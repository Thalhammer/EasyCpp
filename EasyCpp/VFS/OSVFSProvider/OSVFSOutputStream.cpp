#include "OSVFSOutputStream.h"
namespace EasyCpp
{
	namespace VFS
	{
		size_t OSVFSOutputStream::write(const std::vector<uint8_t>& data)
		{
			uint64_t pos = _stream.tellp();
			_stream.write((const char*)data.data(), data.size());
			uint64_t diff = ((uint64_t)_stream.tellp()) - pos;
			_bytesWritten += diff;
			return (size_t)diff;
		}

		uint64_t OSVFSOutputStream::bytesWritten()
		{
			return _bytesWritten;
		}

		bool OSVFSOutputStream::isGood()
		{
			return OSVFSStream::isGood();
		}

		uint64_t OSVFSOutputStream::tell()
		{
			return OSVFSStream::tell();
		}

		void OSVFSOutputStream::seek(uint64_t pos, seek_origin_t origin)
		{
			OSVFSStream::seek(pos, origin);
		}

		bool OSVFSOutputStream::canSeek()
		{
			return OSVFSStream::canSeek();
		}

		OSVFSOutputStream::OSVFSOutputStream()
		{
			_bytesWritten = 0;
		}

		OSVFSOutputStream::OSVFSOutputStream(const std::string& p)
			: OSVFSOutputStream()
		{
			_stream.open(p.c_str(), std::ios::binary | std::ios::out);
			if (!_stream.is_open())
				throw std::runtime_error("Failed to open file");
		}

		OSVFSOutputStream::~OSVFSOutputStream()
		{
		}
	}
}