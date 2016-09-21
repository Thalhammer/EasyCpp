#include "Followers.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Followers::~Followers()
				{
				}

				const std::string& Followers::getHref() const
				{
					return _href;
				}

				uint64_t Followers::getTotal() const
				{
					return _total;
				}

				Followers::Followers()
				{
				}

				AnyValue Followers::toAnyValue() const
				{
					return Bundle({
						{ "href", _href },
						{ "total", _total }
					});
				}

				void Followers::fromAnyValue(const AnyValue & state)
				{
					Bundle followers = state.as<Bundle>();
					_href = followers.get("href").isType<std::string>() ? followers.get<std::string>("href") : "";
					_total = followers.get<uint64_t>("total");
				}
			}
		}
	}
}
