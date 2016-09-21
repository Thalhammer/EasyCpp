#include "TrackLink.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				TrackLink::~TrackLink()
				{
				}

				const std::map<std::string, std::string>& TrackLink::getExternalUrls() const
				{
					return _external_urls;
				}

				const std::string & TrackLink::getHref() const
				{
					return _href;
				}

				const std::string & TrackLink::getID() const
				{
					return _id;
				}

				const std::string & TrackLink::getURI() const
				{
					return _uri;
				}

				AnyValue TrackLink::toAnyValue() const
				{
					return Bundle({
						{ "href", _href },
						{ "id", _id },
						{ "uri", _uri },
						{ "external_urls", Bundle(_external_urls) }
					});
				}

				void TrackLink::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_external_urls = b.get<Bundle>("external_urls").getMap<std::string>();
					_href = b.get<std::string>("href");
					_id = b.get<std::string>("id");
					_uri = b.get<std::string>("uri");
				}

				TrackLink::TrackLink()
				{
				}
			}
		}
	}
}