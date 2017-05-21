#pragma once
#include <vector>
#include <string>
#include <cstdint>
#include <array>

namespace EasyCpp
{
	template<typename crc_val, crc_val WIDTH, crc_val Polynomial, crc_val InitialRemainder, crc_val FinalXorValue, bool TReflectData, bool TReflectRemainder>
	class CRC
	{
	public:
		typedef crc_val crc_t;
		static inline crc_t GetCRC(const std::vector<uint8_t>& data)
		{
			CRC crc;
			crc.update(data);
			return crc.finalize();
		}
		static inline crc_t GetCRC(const std::string& str)
		{
			CRC crc;
			crc.update(str);
			return crc.finalize();
		}
		static inline crc_t GetCRC(const uint8_t* data, size_t dlen)
		{
			CRC crc;
			crc.update(data, dlen);
			return crc.finalize();
		}

		CRC()
			: _table(GetTable())
		{
			static_assert(WIDTH >= 8, "Width needs to be at least 8, 5bit crc is not supported.");
			_remainder = (crc_t)InitialRemainder;
		}

		void update(const std::vector<uint8_t>& data)
		{
			this->update(data.data(), data.size());
		}

		void update(const std::string& data)
		{
			this->update((const uint8_t*)data.data(), data.size());
		}

		void update(const uint8_t* idata, size_t dlen)
		{
			for (size_t byte = 0; byte < dlen; byte++)
			{
				uint8_t data = ReflectData(idata[byte]) ^ (_remainder >> (WIDTH - 8));
				_remainder = _table[data] ^ (_remainder << 8);
			}
		}

		crc_t finalize()
		{
			crc_t res = (ReflectRemainder(_remainder) ^ FinalXorValue);
			_remainder = (crc_t)InitialRemainder;
			return res;
		}

	private: // Instance members
		crc_t _remainder;
		const std::array<crc_t, 256>& _table;

	private: // Static functions
		template <bool M = TReflectData, typename std::enable_if<M>::type* = nullptr>
		static inline uint8_t ReflectData(uint8_t data)
		{
			return (uint8_t)Reflect(data, 8);
		}

		template <bool M = TReflectData, typename std::enable_if<!M>::type* = nullptr>
		static inline uint8_t ReflectData(uint8_t data)
		{
			return data;
		}

		template <bool M = TReflectRemainder, typename std::enable_if<M>::type* = nullptr>
		static inline crc_t ReflectRemainder(crc_t data)
		{
			return Reflect(data, WIDTH);
		}

		template <bool M = TReflectRemainder, typename std::enable_if<!M>::type* = nullptr>
		static inline crc_t ReflectRemainder(crc_t data)
		{
			return data;
		}

		static inline crc_t Reflect(crc_t data, uint8_t nBits)
		{
			crc_t  reflection = 0x00000000;
			uint8_t  bit;

			for (bit = 0; bit < nBits; ++bit)
			{
				if (data & 0x01)
					reflection |= (crc_t(1) << ((nBits - 1) - bit));
				data = (data >> 1);
			}

			return reflection;
		}

		static constexpr crc_t TOPBIT = (crc_t(1) << (WIDTH - 1));

		static crc_t TableCalcRemainder(int32_t dividend)
		{
			crc_t remainder = crc_t(dividend) << (WIDTH - 8);
			for (uint8_t bit = 8; bit > 0; bit--)
			{
				remainder = remainder&TOPBIT ? ((remainder << 1) ^ Polynomial) : (remainder << 1);
			}
			return remainder;
		}

		static std::array<crc_t, 256> MakeTable()
		{
			std::array<crc_t, 256> table;
			for (int32_t dividend = 0; dividend < 256; dividend++)
			{
				table[dividend] = TableCalcRemainder(dividend);
			}
			return table;
		}

		static const std::array<crc_t, 256>& GetTable()
		{
			static std::array<crc_t, 256> table = MakeTable();
			return table;
		}
	};

	typedef CRC<uint64_t, 64, 0x42F0E1EBA9EA3693, 0xffffffffffffffff, 0xffffffffffffffff, true, true> CRC_64_XZ;
	typedef CRC<uint64_t, 64, 0xAD93D23594C935A9, 0xffffffffffffffff, 0x0000000000000000, true, true> CRC_64_JONES;
	typedef CRC<uint64_t, 64, 0x000000000000001B, 0x0000000000000000, 0x0000000000000000, true, true> CRC_64;
	typedef CRC<uint32_t, 32, 0x000000AF, 0x00000000, 0x00000000, false, false> CRC_32_XFER;
	typedef CRC<uint32_t, 32, 0x04C11DB7, 0xffffffff, 0x00000000, true, true> CRC_32_JAM;
	typedef CRC<uint32_t, 32, 0x04C11DB7, 0x00000000, 0xffffffff, false, false> CRC_32_POSIX;
	typedef CRC<uint32_t, 32, 0x04C11DB7, 0xffffffff, 0xffffffff, false, false> CRC_32_BZIP2;
	typedef CRC<uint32_t, 32, 0x04C11DB7, 0xffffffff, 0x00000000, false, false> CRC_32_MPEG;
	typedef CRC<uint32_t, 32, 0x1EDC6F41, 0xffffffff, 0xffffffff, true, true> CRC_32C;
	typedef CRC<uint32_t, 32, 0x04C11DB7, 0xffffffff, 0xffffffff, true, true> CRC_32;
	typedef CRC<uint32_t, 24, 0x864CFB, 0xb704ce, 0x000000, false, false> CRC_24;
	typedef CRC<uint16_t, 16, 0x1021, 0x0000, 0x0000, false, false> CRC_16_XMODEM;
	typedef CRC<uint16_t, 16, 0x1021, 0xffff, 0xffff, true, true> CRC_16_X25;
	typedef CRC<uint16_t, 16, 0x1021, 0x0000, 0x0000, true, true> CRC_16_KERMIT;
	typedef CRC<uint16_t, 16, 0x0589, 0x0000, 0x0001, false, false> CRC_16_R;
	typedef CRC<uint16_t, 16, 0x1021, 0xffff, 0x0000, false, false> CRC_16_CCITT;
	typedef CRC<uint16_t, 16, 0x1021, 0xffff, 0xffff, false, false> CRC_16_GENIBUS;
	typedef CRC<uint16_t, 16, 0x8005, 0xffff, 0x0000, true, true> CRC_16_MODBUS;
	typedef CRC<uint16_t, 16, 0x8005, 0xffff, 0xffff, true, true> CRC_16_USB;
	typedef CRC<uint16_t, 16, 0x8005, 0x0000, 0x0000, true, true> CRC_16;
	typedef CRC<uint16_t, 15, 0x4599, 0x0000, 0x0000, false, false> CRC_15;
	typedef CRC<uint16_t, 12, 0x80F, 0x000, 0x000, false, true> CRC_12;
	typedef CRC<uint8_t, 8, 0x31, 0x00, 0x00, true, true> CRC_8_DALLAS;
	typedef CRC<uint8_t, 8, 0x07, 0x00, 0x00, false, false> CRC_8;

	// Alias
	typedef CRC_16_XMODEM CRC_16_ZMODEM;
}