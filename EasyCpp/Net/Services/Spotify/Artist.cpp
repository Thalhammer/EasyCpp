#include "Artist.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Artist::~Artist()
				{
				}

				const std::map<std::string, std::string>& Artist::getExternalUrls() const
				{
					return _external_urls;
				}

				const std::string & Artist::getHref() const
				{
					return _href;
				}

				const std::string & Artist::getID() const
				{
					return _id;
				}

				const std::string & Artist::getName() const
				{
					return _name;
				}

				const std::string & Artist::getUri() const
				{
					return _uri;
				}

				AnyValue Artist::toAnyValue() const
				{
					return Bundle({
						{ "external_urls", Bundle(_external_urls) },
						{ "href", _href },
						{ "id", _id },
						{ "name", _name },
						{ "uri", _uri }
					});
				}

				void Artist::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_external_urls = b.get<Bundle>("external_urls").getMap<std::string>();
					_href = b.get<std::string>("href");
					_id = b.get<std::string>("id");
					_name = b.get<std::string>("name");
					_uri = b.get<std::string>("uri");
				}

				Artist::Artist()
				{
				}

			}
		}
	}
}