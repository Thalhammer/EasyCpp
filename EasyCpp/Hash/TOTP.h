#pragma once
#include <chrono>
#include "HOTP.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT TOTP
		{
		public:
			TOTP(const std::string& key);

			void setEpoch(time_t epoch);
			void setEpoch(std::chrono::system_clock::time_point epoch);
			time_t getEpoch();

			void setInterval(size_t interval);
			size_t getInterval();

			std::string generate(int offset = 0, size_t num_digits = 6);
		private:
			HOTP _hotp;
			time_t _epoch;
			size_t _interval;
		};
	}
}