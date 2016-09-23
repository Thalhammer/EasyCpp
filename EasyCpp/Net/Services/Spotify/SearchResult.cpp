#include "SearchResult.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				SearchResult::SearchResult()
				{
				}

				SearchResult::~SearchResult()
				{
				}

				const Paging<Album>& SearchResult::getAlbums() const
				{
					return _albums;
				}

				const Paging<FullArtist>& SearchResult::getArtists() const
				{
					return _artists;
				}

				const Paging<FullTrack>& SearchResult::getTracks() const
				{
					return _tracks;
				}

				const Paging<Playlist>& SearchResult::getPlaylists() const
				{
					return _playlists;
				}

				AnyValue SearchResult::toAnyValue() const
				{
					return Bundle({
						{"artists", _artists.toAnyValue()},
						{"albums", _albums.toAnyValue()},
						{"tracks", _tracks.toAnyValue()},
						{"playlists", _playlists.toAnyValue()}
					});
				}

				void SearchResult::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					if (b.isSet("artists")) _artists.fromAnyValue(b.get("artists"));
					if (b.isSet("albums")) _albums.fromAnyValue(b.get("albums"));
					if (b.isSet("tracks")) _tracks.fromAnyValue(b.get("tracks"));
					if (b.isSet("playlists")) _playlists.fromAnyValue(b.get("playlists"));
				}
			}
		}
	}
}