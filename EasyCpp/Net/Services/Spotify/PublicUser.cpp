#include "PublicUser.h"
#include "../../../Bundle.h"
#include "../../../AnyArray.h"
#include <cstddef>

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				PublicUser::~PublicUser()
				{
				}

				const std::string & PublicUser::getDisplayName() const
				{
					return _display_name;
				}

				const std::map<std::string, std::string>& PublicUser::getExternalUrls() const
				{
					return _external_urls;
				}

				const Followers & PublicUser::getFollowers() const
				{
					return _followers;
				}

				const std::string & PublicUser::getHref() const
				{
					return _href;
				}

				const std::string& PublicUser::getID() const
				{
					return _id;
				}

				const std::vector<Image>& PublicUser::getImages() const
				{
					return _images;
				}

				const std::string & PublicUser::getURI() const
				{
					return _uri;
				}

				AnyValue PublicUser::toAnyValue() const
				{
					return Bundle({
						{ "display_name", _display_name },
						{ "external_urls", Bundle(_external_urls) },
						{ "followers", _followers.toAnyValue() },
						{ "href", _href },
						{ "id", _id },
						{ "images", toAnyArraySerialize(_images) },
						{ "uri", _uri }
					});
				}

				void PublicUser::fromAnyValue(const AnyValue & state)
				{
					Bundle bundle = state.as<Bundle>();
					_display_name = bundle.get("display_name").isType<nullptr_t>() ? "" : bundle.get<std::string>("display_name");
					_external_urls = bundle.get<Bundle>("external_urls").getMap<std::string>();
					_followers.fromAnyValue(bundle.get("followers"));
					_href = bundle.get<std::string>("href");
					_id = bundle.get<std::string>("id");
					_images = fromAnyArray<Image>(bundle.get<AnyArray>("images"));
					_uri = bundle.get<std::string>("uri");
				}

				PublicUser::PublicUser()
				{
				}
			}
		}
	}
}
