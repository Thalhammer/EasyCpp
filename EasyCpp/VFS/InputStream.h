#pragma once
#include "Stream.h"
#include <vector>

namespace EasyCpp
{
	namespace VFS
	{
		class InputStream : public virtual Stream
		{
		public:
			virtual std::vector<uint8_t> read(size_t len) = 0;
			virtual uint64_t bytesRead() = 0;
		};
		typedef std::shared_ptr<InputStream> InputStreamPtr;
	}
}