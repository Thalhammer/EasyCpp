#pragma once
#include "Hash.h"
#include "../NonCopyable.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT SHA224 :
			public Hash, public NonCopyable
		{
		public:
			SHA224();
			virtual ~SHA224();

			virtual void update(const std::string & str) override;
			virtual std::string final() override;
			/// <summary>Get the blocksize of this hashing algorithm</summary>
			/// <returns>The blocksize</returns>
			virtual size_t blocksize() override;
			virtual size_t outputsize() override;
			virtual void reset() override;

			static std::string getString(const std::string& str);
		private:
			void* sha224;
		};
	}
}