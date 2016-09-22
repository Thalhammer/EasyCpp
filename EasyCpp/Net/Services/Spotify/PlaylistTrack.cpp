#include "PlaylistTrack.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				PlaylistTrack::PlaylistTrack()
				{
				}

				PlaylistTrack::~PlaylistTrack()
				{
				}

				const std::string & PlaylistTrack::getAddedAt() const
				{
					return _added_at;
				}

				const PublicUser & PlaylistTrack::getAddedBy() const
				{
					return _added_by;
				}

				bool PlaylistTrack::getIsLocal() const
				{
					return _is_local;
				}

				const FullTrack & PlaylistTrack::getTrack() const
				{
					return _track;
				}

				AnyValue PlaylistTrack::toAnyValue() const
				{
					return Bundle({
						{"added_at", _added_at},
						{"added_by", _added_by.toAnyValue()},
						{"is_local", _is_local},
						{"track", _track.toAnyValue()}
					});
				}

				void PlaylistTrack::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_added_at = b.get<std::string>("added_at");
					_added_by = b.get<PublicUser>("added_by");
					_is_local = b.get<bool>("is_local");
					_track = b.get<FullTrack>("track");
				}
			}
		}
	}
}