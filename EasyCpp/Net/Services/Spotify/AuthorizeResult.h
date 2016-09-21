#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include <string>
#include <chrono>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT AuthorizeResult : public Serialize::Serializable
				{
				public:
					typedef std::chrono::steady_clock::time_point time_point;

					virtual ~AuthorizeResult();

					const std::string& getAccessToken() const;
					const std::string& getTokenType() const;
					const time_point& getExpiration() const;
					const std::string& getRefreshToken() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					AuthorizeResult();

					std::string _access_token;
					std::string _token_type;
					time_point _expires;
					std::string _refresh_token;
					friend class Authorization;
				};
			}
		}
	}
}
