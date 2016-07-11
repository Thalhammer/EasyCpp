#pragma once
#include <memory>

namespace EasyCpp
{
	namespace VFS
	{
		class Stream
		{
		public:
			typedef enum {
				BEGIN,
				CURRENT,
				END
			} seek_origin_t;
			virtual bool isGood() = 0;
			virtual uint64_t tell() = 0;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) = 0;
			virtual bool canSeek() = 0;
		};
		typedef std::shared_ptr<Stream> StreamPtr;
	}
}