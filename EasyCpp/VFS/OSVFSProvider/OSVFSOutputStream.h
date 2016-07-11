#pragma once
#include "OSVFSStream.h"
#include "../OutputStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		class OSVFSOutputStream : public virtual OSVFSStream, public virtual OutputStream
		{
		public:
			OSVFSOutputStream(const std::string& p);
			virtual ~OSVFSOutputStream();
			// Geerbt über OutputStream
			virtual size_t write(const std::vector<uint8_t>& data) override;
			virtual uint64_t bytesWritten() override;
			// Explicit call to correct function
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		protected:
			OSVFSOutputStream();
		private:
			uint64_t _bytesWritten;
		};
	}
}