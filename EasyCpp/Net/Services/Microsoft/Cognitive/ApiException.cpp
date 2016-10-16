#include "ApiException.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Microsoft
			{
				namespace Cognitive
				{
					ApiException::ApiException(std::string code, std::string message, std::string req_id)
						: RuntimeException(message),  _request_id(req_id), _code(code)
					{
					}

					ApiException::~ApiException()
					{
					}

					const std::string & ApiException::getRequestId() const
					{
						return _request_id;
					}

					const std::string & ApiException::getCode() const
					{
						return _code;
					}
				}
			}
		}
	}
}