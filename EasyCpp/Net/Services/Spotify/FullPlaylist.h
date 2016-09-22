#pragma once
#include "Playlist.h"
#include "Paging.h"
#include "Followers.h"
#include "PlaylistTrack.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT FullPlaylist : public Playlist
				{
				public:
					FullPlaylist();
					virtual ~FullPlaylist();

					const std::string& getDescription() const;
					const Followers& getFollowers() const;
					const Paging<PlaylistTrack> getTracks() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::string _description;
					Followers _followers;
					Paging<PlaylistTrack> _tracks;
				};
			}
		}
	}
}
