#include "SavedAlbum.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				SavedAlbum::SavedAlbum()
				{
				}

				SavedAlbum::~SavedAlbum()
				{
				}

				const std::string & SavedAlbum::getAddedAt() const
				{
					return _added_at;
				}

				const FullAlbum & SavedAlbum::getAlbum() const
				{
					return _album;
				}

				AnyValue SavedAlbum::toAnyValue() const
				{
					return Bundle({
						{"added_at", _added_at},
						{"album", _album.toAnyValue()}
					});
				}

				void SavedAlbum::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_added_at = b.get<std::string>("added_at");
					_album = b.get<FullAlbum>("album");
				}
			}
		}
	}
}