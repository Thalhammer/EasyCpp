#pragma once
#include "Hash.h"
#include "../NonCopyable.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT SHA384 :
			public Hash, public NonCopyable
		{
		public:
			SHA384();
			virtual ~SHA384();

			virtual void update(const std::string & str) override;
			virtual std::string final() override;
			/// <summary>Get the blocksize of this hashing algorithm</summary>
			/// <returns>The blocksize</returns>
			virtual size_t blocksize() = 0;

			static std::string getString(const std::string& str);
		private:
			void* sha384;
		};
	}
}