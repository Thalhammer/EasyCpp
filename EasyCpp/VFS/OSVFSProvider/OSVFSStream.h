#pragma once
#include "../Stream.h"
#include <fstream>

namespace EasyCpp
{
	namespace VFS
	{
		class OSVFSStream : public virtual Stream
		{
		public:
			OSVFSStream();
			virtual ~OSVFSStream();
			// Geerbt über Stream
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		protected:
			std::fstream _stream;
		};
	}
}