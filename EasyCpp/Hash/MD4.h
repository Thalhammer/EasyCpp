#pragma once
#include "Hash.h"
#include "../NonCopyable.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT MD4 :
			public Hash, public NonCopyable
		{
		public:
			MD4();
			virtual ~MD4();

			virtual void update(const std::string & str) override;
			virtual std::string final() override;

			static std::string getString(const std::string& str);
		private:
			void* md4;
		};
	}
}