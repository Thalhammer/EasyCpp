#include "FullTrack.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				FullTrack::~FullTrack()
				{
				}

				const Album & FullTrack::getAlbum() const
				{
					return _album;
				}

				const std::map<std::string, std::string>& FullTrack::getExternalIds() const
				{
					return _external_ids;
				}

				int FullTrack::getPopularity() const
				{
					return _popularity;
				}

				AnyValue FullTrack::toAnyValue() const
				{
					Bundle base = Track::toAnyValue().as<Bundle>();
					base.set("album", _album.toAnyValue());
					base.set("external_ids", Bundle(_external_ids));
					base.set("popularity", _popularity);
					return base;
				}

				void FullTrack::fromAnyValue(const AnyValue & state)
				{
					Track::fromAnyValue(state);
					Bundle b = state.as<Bundle>();
					_album.fromAnyValue(b.get("album"));
					_external_ids = b.get<Bundle>("external_ids").getMap<std::string>();
					_popularity = b.get<int>("popularity");
				}

				FullTrack::FullTrack()
				{
				}
			}
		}
	}
}