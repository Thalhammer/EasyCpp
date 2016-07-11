#include "OSVFSInputStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		std::vector<uint8_t> OSVFSInputStream::read(size_t len)
		{
			std::vector<uint8_t> res;
			res.resize(len);
			auto rlen = _stream.read((char*)res.data(), res.size()).gcount();
			res.resize((size_t)rlen);
			_bytesRead += rlen;
			return std::move(res);
		}

		uint64_t OSVFSInputStream::bytesRead()
		{
			return _bytesRead;
		}

		bool OSVFSInputStream::isGood()
		{
			return OSVFSStream::isGood();
		}

		uint64_t OSVFSInputStream::tell()
		{
			return OSVFSStream::tell();
		}

		void OSVFSInputStream::seek(uint64_t pos, seek_origin_t origin)
		{
			OSVFSStream::seek(pos, origin);
		}

		bool OSVFSInputStream::canSeek()
		{
			return OSVFSStream::canSeek();
		}

		OSVFSInputStream::OSVFSInputStream()
		{
			_bytesRead = 0;
		}

		OSVFSInputStream::OSVFSInputStream(const std::string& p)
			: OSVFSInputStream()
		{
			_stream.open(p.c_str(), std::ios::binary | std::ios::in);
			if (!_stream.is_open())
				throw std::runtime_error("Failed to open file");
		}

		OSVFSInputStream::~OSVFSInputStream()
		{
		}
	}
}