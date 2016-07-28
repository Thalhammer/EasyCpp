#pragma once
#include "InputStream.h"
#include "../DllExport.h"

namespace EasyCpp
{
	namespace VFS
	{
		class DLL_EXPORT BinaryReader
		{
		public:
			BinaryReader(InputStreamPtr stream);

			std::vector<uint8_t> readBytes(size_t len);
			int8_t readInt8() { return readPrimitive<int8_t>(); }
			int16_t readInt16() { return readPrimitive<int16_t>(); }
			int32_t readInt32() { return readPrimitive<int32_t>(); }
			int64_t readInt64() { return readPrimitive<int64_t>(); }
			uint8_t readUInt8() { return readPrimitive<uint8_t>(); }
			uint16_t readUInt16() { return readPrimitive<uint16_t>(); }
			uint32_t readUInt32() { return readPrimitive<uint32_t>(); }
			uint64_t readUInt64() { return readPrimitive<uint64_t>(); }
			double readDouble() { return readPrimitive<double>(); }
			float readFloat() { return readPrimitive<float>(); }

			InputStreamPtr getStream();
		private:
			template<typename T>
			T readPrimitive() { return *((T*)this->readBytes(sizeof(T)).data()); }

			InputStreamPtr _stream;
		};
	}
}