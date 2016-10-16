#pragma once
#include "../../../../RuntimeException.h"

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
					class DLL_EXPORT ApiException : public RuntimeException
					{
					public:
						ApiException(std::string code, std::string message, std::string req_id);
						virtual ~ApiException();

						const std::string& getRequestId() const;
						const std::string& getCode() const;
					private:
						std::string _request_id;
						std::string _code;
					};
				}
			}
		}
	}
}
