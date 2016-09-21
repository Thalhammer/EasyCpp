#include "Album.h"
#include <cstddef>
#include "../../../Bundle.h"
#include "../../../AnyArray.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Album::~Album()
				{
				}

				const std::string & Album::getAlbumType()
				{
					return _album_type;
				}

				const std::vector<std::string>& Album::getAvailableMarkets()
				{
					return _available_markets;
				}

				const std::map<std::string, std::string>& Album::getExternalUrls()
				{
					return _external_urls;
				}

				const std::string & Album::getHref()
				{
					return _href;
				}

				const std::string & Album::getID()
				{
					return _id;
				}

				const std::vector<Image>& Album::getImages()
				{
					return _images;
				}

				const std::string& Album::getName()
				{
					return _name;
				}

				const std::string& Album::getURI()
				{
					return _uri;
				}

				Album::Album()
				{
				}

				AnyValue Album::toAnyValue() const
				{
					return Bundle({
						{ "album_type", _album_type },
						{ "available_markets", toAnyArray(_available_markets) },
						{ "external_urls", Bundle(_external_urls) },
						{ "href", _href },
						{ "id", _id },
						{ "images", toAnyArraySerialize(_images) },
						{ "name", _name },
						{ "uri", _uri }
					});
				}

				void Album::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_album_type = b.get<std::string>("album_type");
					if (!b.get("available_markets").isType<nullptr_t>())
						_available_markets = fromAnyArray<std::string>(b.get<AnyArray>("available_markets"));
					_external_urls = b.get<Bundle>("external_urls").getMap<std::string>();
					_href = b.get<std::string>("href");
					_id = b.get<std::string>("id");
					_images = fromAnyArray<Image>(b.get<AnyArray>("images"));
					_name = b.get<std::string>("name");
					_uri = b.get<std::string>("uri");
				}
			}
		}
	}
}
