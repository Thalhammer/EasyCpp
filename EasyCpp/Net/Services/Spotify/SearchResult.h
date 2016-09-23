#pragma once
#include "../../../DllExport.h"
#include "../../../Serialize/Serializable.h"
#include <string>
#include "Paging.h"
#include "Album.h"
#include "FullTrack.h"
#include "FullArtist.h"
#include "Playlist.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT SearchResult : public Serialize::Serializable
				{
				public:
					SearchResult();
					virtual ~SearchResult();

					const Paging<Album>& getAlbums() const;
					const Paging<FullArtist>& getArtists() const;
					const Paging<FullTrack>& getTracks() const;
					const Paging<Playlist>& getPlaylists() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					Paging<Album> _albums;
					Paging<FullArtist> _artists;
					Paging<FullTrack> _tracks;
					Paging<Playlist> _playlists;
				};
			}
		}
	}
}
