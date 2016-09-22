#include "FeaturedPlaylists.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				FeaturedPlaylists::FeaturedPlaylists()
				{
				}
				FeaturedPlaylists::~FeaturedPlaylists()
				{
				}
				const std::string & FeaturedPlaylists::getMessage() const
				{
					return _message;
				}
				const Paging<Playlist>& FeaturedPlaylists::getPlaylists() const
				{
					return _playlists;
				}
				AnyValue FeaturedPlaylists::toAnyValue() const
				{
					return Bundle({
						{"message", _message},
						{"playlists", _playlists.toAnyValue() }
					});
				}
				void FeaturedPlaylists::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_message = b.get<std::string>("message");
					_playlists = b.get<Paging<Playlist>>("playlists");
				}
			}
		}
	}
}