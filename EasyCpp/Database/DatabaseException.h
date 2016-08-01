#pragma once
#include "../RuntimeException.h"
#include "../Bundle.h"

namespace EasyCpp
{
	namespace Database
	{
		class DLL_EXPORT DatabaseException: public RuntimeException
		{
		public:
			/// <summary>Create a new instance using specified code and info.</summary>
			DatabaseException(const std::string& code, const Bundle& info);
			virtual ~DatabaseException();

			/// <summary>Get code.</summary>
			const std::string& getCode() const;
			/// <summary>Get info.</summary>
			const Bundle& getInfo() const;
		private:
			std::string _code;
			Bundle _info;
		};
	}
}