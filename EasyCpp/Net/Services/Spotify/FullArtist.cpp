#include "FullArtist.h"
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
				FullArtist::~FullArtist()
				{
				}

				const Followers & FullArtist::getFollowers() const
				{
					return _followers;
				}

				const std::vector<std::string>& FullArtist::getGenres() const
				{
					return _genres;
				}

				const std::vector<Image>& FullArtist::getImages() const
				{
					return _images;
				}

				int FullArtist::getPopularity() const
				{
					return _popularity;
				}

				AnyValue FullArtist::toAnyValue() const
				{
					Bundle parent = Artist::toAnyValue().as<Bundle>();
					parent.set("followers", _followers.toAnyValue());
					parent.set("genres", toAnyArray(_genres));
					parent.set("images", toAnyArraySerialize(_images));
					parent.set("popularity", _popularity);
					return parent;
				}

				void FullArtist::fromAnyValue(const AnyValue & state)
				{
					Artist::fromAnyValue(state);
					Bundle b = state.as<Bundle>();
					_followers.fromAnyValue(b.get("followers"));
					_genres = fromAnyArray<std::string>(b.get<AnyArray>("genres"));
					_images = fromAnyArray<Image>(b.get<AnyArray>("images"));
					_popularity = b.get<int>("popularity");
				}

				FullArtist::FullArtist()
				{
				}
			}
		}
	}
}