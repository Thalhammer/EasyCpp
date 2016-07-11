#pragma once
#include "Stream.h"
#include <vector>

namespace EasyCpp
{
	namespace VFS
	{
		class OutputStream : public virtual Stream
		{
		public:
			virtual size_t write(const std::vector<uint8_t>& data) = 0;
			virtual uint64_t bytesWritten() = 0;
		};
		typedef std::shared_ptr<OutputStream> OutputStreamPtr;
	}
}
