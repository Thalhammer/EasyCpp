#include "OSVFSStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		OSVFSStream::OSVFSStream()
		{
		}

		OSVFSStream::~OSVFSStream()
		{
			_stream.close();
		}

		bool OSVFSStream::isGood()
		{
			return _stream.good();
		}

		uint64_t OSVFSStream::tell()
		{
			return _stream.tellg();
		}

		void OSVFSStream::seek(uint64_t pos, seek_origin_t origin)
		{
			switch (origin)
			{
			case BEGIN:
				_stream.seekg(pos, std::ios_base::beg); break;
			case CURRENT:
				_stream.seekg(pos, std::ios_base::cur); break;
			case END:
				_stream.seekg(pos, std::ios_base::end); break;
			default:
				throw std::runtime_error("Invalid seek origin");
			}
		}

		bool OSVFSStream::canSeek()
		{
			return true;
		}
	}
}
