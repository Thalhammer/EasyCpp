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
			DatabaseException(const std::string& code, const Bundle& info);
			virtual ~DatabaseException();

			const std::string& getCode() const;
			const Bundle& getInfo() const;
		private:
			std::string _code;
			Bundle _info;
		};
	}
}