#pragma once
#include "Hash.h"
#include "../NonCopyable.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT SHA512 :
			public Hash, public NonCopyable
		{
		public:
			SHA512();
			virtual ~SHA512();

			virtual void update(const std::string & str) override;
			virtual std::string final() override;

			static std::string getString(const std::string& str);
		private:
			void* sha512;
		};
	}
}