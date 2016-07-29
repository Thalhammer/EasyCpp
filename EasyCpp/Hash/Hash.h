#pragma once
#include <string>
#include <memory>
#include "../DllExport.h"

namespace EasyCpp
{
	namespace Hash
	{
		class DLL_EXPORT Hash
		{
		public:
			virtual ~Hash() {};
			/// <summary>Update the hashvalue using provided data.</summary>
			/// <param name="str">Data to hash</param>
			virtual void update(const std::string& str) = 0;
			/// <summary>Finalize hash value and return the result as a string</summary>
			/// <returns>The hash value</returns>
			virtual std::string final() = 0;
			/// <summary>Get the blocksize of this hashing algorithm</summary>
			/// <returns>The blocksize</returns>
			virtual size_t blocksize() = 0;
		};
		typedef std::shared_ptr<Hash> HashPtr;
	}
}