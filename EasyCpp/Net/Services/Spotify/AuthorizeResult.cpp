#include "AuthorizeResult.h"
#include "../../../Bundle.h"
#include <cstddef>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				AuthorizeResult::~AuthorizeResult()
				{
				}

				const std::string & AuthorizeResult::getAccessToken() const
				{
					return _access_token;
				}

				const std::string & AuthorizeResult::getTokenType() const
				{
					return _token_type;
				}

				const AuthorizeResult::time_point& AuthorizeResult::getExpiration() const
				{
					return _expires;
				}

				const std::string & AuthorizeResult::getRefreshToken() const
				{
					return _refresh_token;
				}

				AuthorizeResult::AuthorizeResult()
				{
				}

				AnyValue AuthorizeResult::toAnyValue() const
				{
					auto expires = std::chrono::duration_cast<std::chrono::seconds>(_expires - std::chrono::steady_clock::now()).count();
					return Bundle({
						{ "access_token", _access_token },
						{ "token_type", _token_type },
						{ "expires_in", expires },
						{ "refresh_token", _refresh_token }
					});
				}

				void AuthorizeResult::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_access_token = b.get<std::string>("access_token");
					_token_type = b.get<std::string>("token_type");
					_expires = std::chrono::steady_clock::now() + std::chrono::seconds(b.get<int>("expires_in"));
					_refresh_token = b.get("refresh_token").isType<nullptr_t>() ? "" : b.get<std::string>("refresh_token");
				}
			}
		}
	}
}
