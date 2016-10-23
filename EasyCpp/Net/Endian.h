#pragma once
#include "../DllExport.h"
#include <cstddef>
#include <cstdint>

namespace EasyCpp
{
	namespace Net
	{
		class DLL_EXPORT Endian
		{
		public:
			static uint8_t hton(uint8_t val)
			{
				return val;
			}
			static uint16_t hton(uint16_t val)
			{
				cv_16 cv;
				cv.val = val;
				return (uint16_t)cv.b1 << 8 | cv.b2;
			}
			static uint32_t hton(uint32_t val)
			{
				cv_32 cv;
				cv.val = val;
				return (uint32_t)cv.b1 << 24 | (uint32_t)cv.b2 << 16 | (uint32_t)cv.b3 << 8 | cv.b4;
			}
			static uint64_t hton(uint64_t val)
			{
				cv_64 cv;
				cv.val = val;
				return (uint64_t)cv.b1 << 56 | (uint64_t)cv.b2 << 48 | (uint64_t)cv.b3 << 40 | (uint64_t)cv.b4 << 32 
					| (uint64_t)cv.b5 << 24 | (uint64_t)cv.b6 << 16 | (uint64_t)cv.b7 << 8 | (uint64_t)cv.b8;
			}
			static uint8_t ntoh(uint8_t val)
			{
				return hton(val);
			}
			static uint16_t ntoh(uint16_t val)
			{
				return hton(val);
			}
			static uint32_t ntoh(uint32_t val)
			{
				return hton(val);
			}
			static uint64_t ntoh(uint64_t val)
			{
				return hton(val);
			}
		private:
			union cv_16 {
				struct {
					uint8_t b1;
					uint8_t b2;
				};
				uint16_t val;
			};
			union cv_32 {
				struct {
					uint8_t b1;
					uint8_t b2;
					uint8_t b3;
					uint8_t b4;
				};
				uint32_t val;
			};
			union cv_64 {
				struct {
					uint8_t b1;
					uint8_t b2;
					uint8_t b3;
					uint8_t b4;
					uint8_t b5;
					uint8_t b6;
					uint8_t b7;
					uint8_t b8;
				};
				uint64_t val;
			};
		};
	}
}