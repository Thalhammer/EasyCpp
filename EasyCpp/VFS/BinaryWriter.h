#pragma once
#include "OutputStream.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT BinaryWriter
		{
		public:
			BinaryWriter(OutputStreamPtr stream);

			void writeInt8(int8_t v) { this->writeBytes(&v, sizeof(int8_t)); }
			void writeInt16(int16_t v) { this->writeBytes(&v, sizeof(int16_t)); }
			void writeInt32(int32_t v) { this->writeBytes(&v, sizeof(int32_t)); }
			void writeInt64(int64_t v) { this->writeBytes(&v, sizeof(int64_t)); }
			void writeUInt8(uint8_t v) { this->writeBytes(&v, sizeof(uint8_t)); }
			void writeUInt16(uint16_t v) { this->writeBytes(&v, sizeof(uint16_t)); }
			void writeUInt32(uint32_t v) { this->writeBytes(&v, sizeof(uint32_t)); }
			void writeUInt64(uint64_t v) { this->writeBytes(&v, sizeof(uint64_t)); }
			void writeDouble(double v) { this->writeBytes(&v, sizeof(double)); }
			void writeFloat(float v) { this->writeBytes(&v, sizeof(float)); }

			void writeBytes(const void* ptr, size_t len);
			void writeString(const std::string& str) { this->writeBytes((const uint8_t*)str.data(), str.size()); }
			void writeCString(const std::string& str) { writeString(str); writeUInt8(0x00); }

			OutputStreamPtr getStream();
		private:
			OutputStreamPtr _stream;
		};
	}
}