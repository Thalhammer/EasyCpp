#pragma once
#include "OSVFSStream.h"
#include "../InputStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		class OSVFSInputStream : public virtual OSVFSStream, public virtual InputStream
		{
		public:
			OSVFSInputStream(const std::string& p);
			virtual ~OSVFSInputStream();
			// Geerbt über InputStream
			virtual std::vector<uint8_t> read(size_t len) override;
			virtual uint64_t bytesRead() override;
			// Explicit call to correct function
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		protected:
			OSVFSInputStream();
		private:
			uint64_t _bytesRead;
		};
	}
}