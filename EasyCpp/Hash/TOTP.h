#pragma once
#include <chrono>
#include "HOTP.h"

namespace EasyCpp
{
	namespace Hash
	{
		/// <summary>Time based One Time Password generator.</summary>
		class DLL_EXPORT TOTP
		{
		public:
			/// <summary>Instanciate TOTP with given key.</summary>
			TOTP(const std::string& key);

			/// <summary>Set epoch to use.</summary>
			void setEpoch(time_t epoch);
			/// <summary>Set epoch using std::chrono.</summary>
			void setEpoch(std::chrono::system_clock::time_point epoch);
			/// <summary>Get current epoch.</summary>
			time_t getEpoch();

			/// <summary>Set Interval to use.</summary>
			void setInterval(size_t interval);
			/// <summary>Get current interval.</summary>
			size_t getInterval();

			/// <summary>Generate a token of num_digits length for current interval.</summary>
			/// You can generate last and next token by adjusting offset to -1 or +1.
			std::string generate(int offset = 0, size_t num_digits = 6);
		private:
			HOTP _hotp;
			time_t _epoch;
			size_t _interval;
		};
	}
}