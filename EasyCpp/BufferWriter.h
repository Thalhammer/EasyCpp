#pragma once
#include <vector>
#include <cstdint>

namespace EasyCpp
{
	class BufferWriter
	{
	public:

		BufferWriter(std::vector<uint8_t>& vect) : _buffer(vect), _pos(0), _overwrite(true) {}

		virtual ~BufferWriter() {}

		void setPosition(size_t pos)
		{
			if (pos > _buffer.size())
				throw std::invalid_argument("New position exceeds Bufferbounds");

			_pos = pos;
		}
		size_t getPosition() const { return _pos; }
		size_t getSize() const { return _buffer.size(); }

		void setOverwrite(bool overwrite) { _overwrite = overwrite; }
		bool isOverwrite() const { return _overwrite; }

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

		void writeBytes(const void* ptr, size_t len)
		{
			if (_overwrite)
			{
				// Expand buffer if needed
				if (_buffer.size() < _pos + len)
					_buffer.resize(_pos + len);

				for (size_t i = 0; i < len; i++)
				{
					_buffer[_pos + i] = ((const uint8_t*)ptr)[i];
				}
				_pos += len;
			}
			else {
				_buffer.insert(_buffer.begin() + _pos, (const uint8_t*)ptr, ((const uint8_t*)ptr) + len);
			}
		}
		void writeString(const std::string& str) { this->writeBytes((const uint8_t*)str.data(), str.size()); }
		void writeCString(const std::string& str) { writeString(str); writeUInt8(0x00); }
	private:
		std::vector<uint8_t>& _buffer;

		size_t _pos;
		bool _overwrite;
	};
}
