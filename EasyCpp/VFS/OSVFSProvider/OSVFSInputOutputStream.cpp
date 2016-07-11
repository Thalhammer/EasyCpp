#include "OSVFSInputOutputStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		std::vector<uint8_t> OSVFSInputOutputStream::read(size_t len)
		{
			return OSVFSInputStream::read(len);
		}

		uint64_t OSVFSInputOutputStream::bytesRead()
		{
			return OSVFSInputStream::bytesRead();
		}

		bool OSVFSInputOutputStream::isGood()
		{
			return OSVFSStream::isGood();
		}

		uint64_t OSVFSInputOutputStream::tell()
		{
			return OSVFSStream::tell();
		}

		void OSVFSInputOutputStream::seek(uint64_t pos, seek_origin_t origin)
		{
			OSVFSStream::seek(pos, origin);
		}

		bool OSVFSInputOutputStream::canSeek()
		{
			return OSVFSStream::canSeek();
		}

		OSVFSInputOutputStream::OSVFSInputOutputStream(const std::string& p)
		{
			_stream.open(p.c_str(), std::ios::binary | std::ios::in | std::ios::out);
			if (!_stream.is_open())
				throw std::runtime_error("Failed to open file");
		}

		OSVFSInputOutputStream::~OSVFSInputOutputStream()
		{
		}

		size_t OSVFSInputOutputStream::write(const std::vector<uint8_t>& data)
		{
			return OSVFSOutputStream::write(data);
		}

		uint64_t OSVFSInputOutputStream::bytesWritten()
		{
			return OSVFSOutputStream::bytesWritten();
		}
	}
}