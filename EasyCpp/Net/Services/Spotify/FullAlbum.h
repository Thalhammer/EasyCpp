#pragma once
#include "Album.h"
#include "Image.h"
#include "Copyright.h"
#include "Artist.h"
#include "Paging.h"
#include "Track.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT FullAlbum : public Album
				{
				public:
					FullAlbum();
					virtual ~FullAlbum();

					const std::vector<Artist>& getArtists() const;
					const std::vector<Copyright>& getCopyright() const;
					const std::map<std::string, std::string>& getExternalIds() const;
					const std::vector<std::string>& getGenres() const;
					int getPopularity() const;
					const std::string& getReleaseDate() const;
					const std::string& getReleaseDatePrecision() const;
					const Paging<Track>& getTracks() const;

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override;
					virtual void fromAnyValue(const AnyValue & state) override;
				private:
					std::vector<Artist> _artists;
					std::vector<Copyright> _copyrights;
					std::map<std::string, std::string> _external_ids;
					std::vector<std::string> _genres;
					int _popularity;
					std::string _release_date;
					std::string _release_date_precision;
					Paging<Track> _tracks;
				};

			}
		}
	}
}
