#pragma once
#include <cstdint>
#include <vector>

namespace EasyCpp
{
	/// <summary>Allows easy reading of binary structures.</summary>
	class BufferReader
	{
	public:
		//! \brief Erstellt einen BufferReader vom angebenen Vektor.
		BufferReader(const std::vector<uint8_t>& vect) : _buffer(vect) {}
		virtual ~BufferReader() {}

		//! Gibt die Größe des Buffers zurück.
		//! \return Gesamte Buffergröße
		size_t getSize() const { return _buffer.size(); }
		//! Gibt die aktuelle Position im Buffer zurück.
		//! \return Aktuelle Position
		size_t getPosition() const { return _pos; }
		//! Setzt die aktuelle Position auf einen neuen Wert.
		//! \param pos Die gewünschte Position im Buffer.
		void setPosition(size_t pos)
		{
			if (pos > _buffer.size())
				throw std::invalid_argument("Position must be within buffer bounds");

			_pos = pos;
		}
		//! Liest die angegebene Anzahl Bytes aus dem Buffer.
		//! \param len Gewünschte Anzahl Bytes
		//! \return Pointer auf die Startadresse innerhalb des Buffers.
		const uint8_t* readBytes(size_t len)
		{
			if (_pos + len > _buffer.size())
				throw std::invalid_argument("Requested read operation exceeds available bytes");

			const uint8_t* ptr = _buffer.data() + _pos;
			_pos += len;
			return ptr;
		}

		//! Liest einen Vorzeichenbehafteten 8bit Wert aus dem Buffer.
		//! \return Der 8bit Wert an der aktuellen position.
		int8_t readInt8() { return *((int8_t*)this->readBytes(sizeof(int8_t))); }
		//! Liest einen Vorzeichenbehafteten 16bit Wert aus dem Buffer.
		//! \return Der 16bit Wert an der aktuellen position.
		int16_t readInt16() { return *((int16_t*)this->readBytes(sizeof(int16_t))); }
		//! Liest einen Vorzeichenbehafteten 32bit Wert aus dem Buffer.
		//! \return Der 32bit Wert an der aktuellen position.
		int32_t readInt32() { return *((int32_t*)this->readBytes(sizeof(int32_t))); }
		//! Liest einen Vorzeichenbehafteten 64bit Wert aus dem Buffer.
		//! \return Der 64bit Wert an der aktuellen position.
		int64_t readInt64() { return *((int64_t*)this->readBytes(sizeof(int64_t))); }

		//! Liest einen Vorzeichenlosen 8bit Wert aus dem Buffer.
		//! \return Der 8bit Wert an der aktuellen position.
		uint8_t readUInt8() { return *((uint8_t*)this->readBytes(sizeof(uint8_t))); }
		//! Liest einen Vorzeichenlosen 16bit Wert aus dem Buffer.
		//! \return Der 16bit Wert an der aktuellen position.
		uint16_t readUInt16() { return *((uint16_t*)this->readBytes(sizeof(uint16_t))); }
		//! Liest einen Vorzeichenlosen 32bit Wert aus dem Buffer.
		//! \return Der 32bit Wert an der aktuellen position.
		uint32_t readUInt32() { return *((uint32_t*)this->readBytes(sizeof(uint32_t))); }
		//! Liest einen Vorzeichenlosen 64bit Wert aus dem Buffer.
		//! \return Der 64bit Wert an der aktuellen position.
		uint64_t readUInt64() { return *((uint64_t*)this->readBytes(sizeof(uint64_t))); }

		//! Liest einen Gleitkommawert mit doppelter Genauigkeit aus dem Buffer.
		//! \return Der double Wert an der aktuellen position.
		double readDouble() { return *((double*)this->readBytes(sizeof(double))); }
		//! Liest einen Gleitkommawert mit einfacher Genauigkeit aus dem Buffer.
		//! \return Der float Wert an der aktuellen position.
		float readFloat() { return *((float*)this->readBytes(sizeof(float))); }

		//! Liest eine Anzahl Bytes aus dem Buffer die aktuelle Position zu erhöhen.
		//! \param len Gewünschte Anzahl Bytes
		//! \return Ein Pointer auf die Startadresse der angeforderten Bytes.
		const uint8_t* peekBytes(size_t len) const
		{
			if (_pos + len > _buffer.size())
				throw std::invalid_argument("Requested read operation exceeds available bytes");

			const uint8_t* ptr = _buffer.data() + _pos;
			return ptr;
		}

		//! Liest einen Vorzeichenbehafteten 8bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 8bit Wert an der aktuellen position.
		int8_t peekInt8() const { return *((int8_t*)this->peekBytes(sizeof(int8_t))); }
		//! Liest einen Vorzeichenbehafteten 16bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 16bit Wert an der aktuellen position.
		int16_t peekInt16() const { return *((int16_t*)this->peekBytes(sizeof(int16_t))); }
		//! Liest einen Vorzeichenbehafteten 32bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 32bit Wert an der aktuellen position.
		int32_t peekInt32() const { return *((int32_t*)this->peekBytes(sizeof(int32_t))); }
		//! Liest einen Vorzeichenbehafteten 64bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 64bit Wert an der aktuellen position.
		int64_t peekInt64() const { return *((int64_t*)this->peekBytes(sizeof(int64_t))); }

		//! Liest einen Vorzeichenlosen 8bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 8bit Wert an der aktuellen position.
		uint8_t peekUInt8() const { return *((uint8_t*)this->peekBytes(sizeof(uint8_t))); }
		//! Liest einen Vorzeichenlosen 16bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 16bit Wert an der aktuellen position.
		uint16_t peekUInt16() const { return *((uint16_t*)this->peekBytes(sizeof(uint16_t))); }
		//! Liest einen Vorzeichenlosen 32bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 32bit Wert an der aktuellen position.
		uint32_t peekUInt32() const { return *((uint32_t*)this->peekBytes(sizeof(uint32_t))); }
		//! Liest einen Vorzeichenlosen 64bit Wert aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der 64bit Wert an der aktuellen position.
		uint64_t peekUInt64() const { return *((uint64_t*)this->peekBytes(sizeof(uint64_t))); }

		//! Liest einen Gleitkommawert mit doppelter Genauigkeit aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der double Wert an der aktuellen position.
		double peekDouble() const { return *((double*)this->peekBytes(sizeof(double))); }
		//! Liest einen Gleitkommawert mit einfacher Genauigkeit aus dem Buffer ohne die aktuelle Position zu erhöhen.
		//! \return Der float Wert an der aktuellen position.
		float peekFloat() const { return *((float*)this->peekBytes(sizeof(float))); }

		//! Liest einen C string an der aktuellen Position aus dem Buffer.
		//! \return Der gelesene String.
		std::string readCString()
		{
			std::string result;
			uint8_t c = this->readUInt8();
			while (c != 0x00)
			{
				result += c;
				c = readUInt8();
			}
			return result;
		}

		//! Liest einen String mit der angegebenen Länge aus dem Buffer.
		//! \return Der gelesene String.
		std::string readString(size_t len)
		{
			const uint8_t* ptr = this->readBytes(len);
			return std::string(ptr, ptr + len);
		}
	private:
		const std::vector<uint8_t>& _buffer;

		size_t _pos = 0;
	};
}
