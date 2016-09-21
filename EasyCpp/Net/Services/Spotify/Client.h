#pragma once
#include "../../../DllExport.h"
#include <string>
#include "PublicUser.h"
#include "User.h"
#include "FullAlbum.h"
#include "FullArtist.h"
#include "FullTrack.h"
#include "AudioFeatures.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				class DLL_EXPORT Client
				{
				public:

					Client();
					virtual ~Client();

					void setAccessToken(const std::string& str);
					void setMarket(const std::string& str);

					User getCurrentUser();
					PublicUser getUser(const std::string& name);

					FullAlbum getAlbum(const std::string& uri);
					std::vector<FullAlbum> getAlbums(const std::vector<std::string>& uris);
					Paging<Track> getAlbumTracks(const std::string& album_uri, int limit = 20, int offset = 0);

					FullArtist getArtist(const std::string& uri);
					std::vector<FullArtist> getArtists(const std::vector<std::string>& uris);
					Paging<Album> getArtistsAlbums(const std::string& uri, const std::vector<std::string>& album_type = { "album", "single", "appears_on", "compilation" }, int limit = 20, int offset = 0);
					std::vector<FullTrack> getArtistsTopTracks(const std::string& uri);
					std::vector<FullArtist> getArtistsRelatedArtists(const std::string& uri);

					FullTrack getTrack(const std::string& uri);
					std::vector<FullTrack> getTracks(const std::vector<std::string>& uris);

					AudioFeatures getAudioFeatures(const std::string& uri);
					std::vector<AudioFeatures> getAudioFeatures(const std::vector<std::string>& uris);

				private:
					std::string _token;
					std::string _basepath;

					std::string _market;

					Bundle doGET(const std::string& url, bool req_auth = true);
				};
			}
		}
	}
}