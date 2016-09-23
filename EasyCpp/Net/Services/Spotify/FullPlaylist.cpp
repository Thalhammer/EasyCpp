#include "FullPlaylist.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				FullPlaylist::FullPlaylist()
				{
				}

				FullPlaylist::~FullPlaylist()
				{
				}

				const std::string & FullPlaylist::getDescription() const
				{
					return _description;
				}

				const Followers & FullPlaylist::getFollowers() const
				{
					return _followers;
				}

				const Paging<PlaylistTrack> FullPlaylist::getTracks() const
				{
					return _tracks;
				}

				AnyValue FullPlaylist::toAnyValue() const
				{
					Bundle b = Playlist::toAnyValue().as<Bundle>();
					b.set("description", _description);
					b.set("followers", _followers.toAnyValue());
					b.set("tracks", _tracks.toAnyValue());
					return b;
				}

				void FullPlaylist::fromAnyValue(const AnyValue & state)
				{
					Playlist::fromAnyValue(state);
					Bundle b = state.as<Bundle>();
					_description = b.get("description").isType<nullptr_t>() ? "" : b.get<std::string>("description");
					_followers = b.get<Followers>("followers");
					_tracks = b.get<Paging<PlaylistTrack>>("tracks");
				}
			}
		}
	}
}