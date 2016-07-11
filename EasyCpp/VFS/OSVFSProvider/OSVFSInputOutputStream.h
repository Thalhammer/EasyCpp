#pragma once
#include "OSVFSOutputStream.h"
#include "OSVFSInputStream.h"
#include "../InputOutputStream.h"

namespace EasyCpp
{
	namespace VFS
	{
		class OSVFSInputOutputStream : public virtual OSVFSInputStream, public virtual OSVFSOutputStream, public virtual InputOutputStream
		{
		public:
			OSVFSInputOutputStream(const std::string& p);
			virtual ~OSVFSInputOutputStream();
			// Geerbt über OutputStream
			virtual size_t write(const std::vector<uint8_t>& data) override;
			virtual uint64_t bytesWritten() override;
			// Geerbt über InputStream
			virtual std::vector<uint8_t> read(size_t len) override;
			virtual uint64_t bytesRead() override;
			// Explicit call to correct function
			virtual bool isGood() override;
			virtual uint64_t tell() override;
			virtual void seek(uint64_t pos, seek_origin_t origin = BEGIN) override;
			virtual bool canSeek() override;
		protected:
		private:
		};
	}
}