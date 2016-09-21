#include "Exception.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Exception::Exception(int code, std::string message)
					: RuntimeException(message), _code(code)
				{
				}

				Exception::~Exception()
				{
				}

				int Exception::getErrorCode()
				{
					return _code;
				}
			}
		}
	}
}