#pragma once
#include "Hash.h"
#include "../NonCopyable.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT SHA1 :
			public Hash, public NonCopyable
		{
		public:
			SHA1();
			virtual ~SHA1();

			virtual void update(const std::string & str) override;
			virtual std::string final() override;

			static std::string getString(const std::string& str);
		private:
			void* sha1;
		};
	}
}