#include "TOTP.h"

namespace EasyCpp
{
	namespace Hash
	{
		TOTP::TOTP(const std::string & key)
			:_hotp(key)
		{
			this->setEpoch(0);
			this->setInterval(30);
		}

		void TOTP::setEpoch(time_t epoch)
		{
			_epoch = epoch;
		}

		void TOTP::setEpoch(std::chrono::system_clock::time_point epoch)
		{
			_epoch = std::chrono::system_clock::to_time_t(epoch);
		}

		time_t TOTP::getEpoch()
		{
			return _epoch;
		}

		void TOTP::setInterval(size_t interval)
		{
			_interval = interval;
		}

		size_t TOTP::getInterval()
		{
			return _interval;
		}

		std::string TOTP::generate(int offset, size_t num_digits)
		{
			uint64_t count = (std::chrono::system_clock::to_time_t(std::chrono::system_clock::now()) - _epoch) / _interval;
			count += offset;
			return _hotp.generate(count, num_digits);
		}
	}
}