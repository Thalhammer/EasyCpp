#include "SavedTrack.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				SavedTrack::SavedTrack()
				{
				}

				SavedTrack::~SavedTrack()
				{
				}

				const std::string & SavedTrack::getAddedAt() const
				{
					return _added_at;
				}

				const FullTrack & SavedTrack::getTrack() const
				{
					return _track;
				}

				AnyValue SavedTrack::toAnyValue() const
				{
					return Bundle({
						{ "added_at", _added_at },
						{ "album", _track.toAnyValue() }
					});
				}

				void SavedTrack::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_added_at = b.get<std::string>("added_at");
					_track = b.get<FullTrack>("track");
				}
			}
		}
	}
}