#pragma once
#include "../../../RuntimeException.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Exception : public RuntimeException
				{
				public:
					Exception(int code, std::string message);
					virtual ~Exception();
					int getErrorCode();
				private:
					int _code;
				};

			}
		}
	}
}