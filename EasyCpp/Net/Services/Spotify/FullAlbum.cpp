#include "FullAlbum.h"
#include "../../../AnyArray.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				FullAlbum::~FullAlbum()
				{
				}

				const std::vector<Artist>& FullAlbum::getArtists() const
				{
					return _artists;
				}

				const std::vector<Copyright>& FullAlbum::getCopyright() const
				{
					return _copyrights;
				}

				const std::map<std::string, std::string>& FullAlbum::getExternalIds() const
				{
					return _external_ids;
				}

				const std::vector<std::string>& FullAlbum::getGenres() const
				{
					return _genres;
				}

				int FullAlbum::getPopularity() const
				{
					return _popularity;
				}

				const std::string & FullAlbum::getReleaseDate() const
				{
					return _release_date;
				}

				const std::string & FullAlbum::getReleaseDatePrecision() const
				{
					return _release_date_precision;
				}

				const Paging<Track>& FullAlbum::getTracks() const
				{
					return _tracks;
				}

				AnyValue FullAlbum::toAnyValue() const
				{
					Bundle parent = Album::toAnyValue().as<Bundle>();
					parent.set("artists", toAnyArraySerialize(_artists));
					parent.set("copyrights", toAnyArraySerialize(_copyrights));
					parent.set("external_ids", Bundle(_external_ids));
					parent.set("genres", toAnyArray(_genres));
					parent.set("popularity", _popularity);
					parent.set("release_date", _release_date);
					parent.set("release_date_precision", _release_date_precision);
					parent.set("tracks", _tracks.toAnyValue());
					return parent;
				}
				void FullAlbum::fromAnyValue(const AnyValue & state)
				{
					Album::fromAnyValue(state);
					Bundle b = state.as<Bundle>();
					_artists = fromAnyArray<Artist>(b.get<AnyArray>("artists"));
					_copyrights = fromAnyArray<Copyright>(b.get<AnyArray>("copyrights"));
					_external_ids = b.get<Bundle>("external_ids").getMap<std::string>();
					_genres = fromAnyArray<std::string>(b.get<AnyArray>("genres"));
					_popularity = b.get<int>("popularity");
					_release_date = b.get<std::string>("release_date");
					_release_date_precision = b.get<std::string>("release_date_precision");
					_tracks.fromAnyValue(b.get("tracks"));
				}

				FullAlbum::FullAlbum()
				{
				}
			}
		}
	}
}
