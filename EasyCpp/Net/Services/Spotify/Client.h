#pragma once
#include "../../../DllExport.h"
#include <string>
#include "PublicUser.h"
#include "User.h"
#include "FullAlbum.h"
#include "FullArtist.h"
#include "FullTrack.h"
#include "FullPlaylist.h"
#include "AudioFeatures.h"
#include "Category.h"
#include "SavedAlbum.h"
#include "SavedTrack.h"
#include "TimeRange.h"

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
					Paging<Album> getArtistAlbums(const std::string& uri, const std::vector<std::string>& album_type = { "album", "single", "appears_on", "compilation" }, int limit = 20, int offset = 0);
					std::vector<FullTrack> getArtistTopTracks(const std::string& uri);
					std::vector<FullArtist> getArtistRelatedArtists(const std::string& uri);

					FullTrack getTrack(const std::string& uri);
					std::vector<FullTrack> getTracks(const std::vector<std::string>& uris);

					AudioFeatures getAudioFeatures(const std::string& uri);
					std::vector<AudioFeatures> getAudioFeatures(const std::vector<std::string>& uris);

					Paging<Category> getCategoriesList(const std::string& locale, int limit = 20, int offset = 0);
					Category getCategory(const std::string& id, const std::string& locale);
					
					Paging<Album> getNewReleases(int limit = 20, int offset = 0);

					Paging<SavedAlbum> getMySavedAlbums(int limit = 20, int offset = 0);
					Paging<SavedTrack> getMySavedTracks(int limit = 20, int offset = 0);
					Paging<FullTrack> getMyTopTracks(int limit = 20, int offset = 0, TimeRange range = TimeRange::MEDIUM);
					Paging<FullArtist> getMyTopArtists(int limit = 20, int offset = 0, TimeRange range = TimeRange::MEDIUM);
					Paging<FullArtist> getUserFollowedArtists(int limit = 20, int offset = 0);

					FullPlaylist getUserPlaylist(const std::string& uid, const std::string& pid);
					Paging<Playlist> getUserPlaylists(const std::string& uid, int limit = 20, int offset = 0);
					Paging<Playlist> getMyPlaylists(int limit = 20, int offset = 0);
					/* TODO:
					 * addMyAlbums
					 * addMyTracks
					 * addUserPlaylistTracks
					 * createUserPlaylist
					 * currentUserFollows
					 * deleteMyAlbums
					 * deleteMyTracks
					 * deleteUserPlaylistTracks
					 * followArtist
					 * unfollowArtist
					 * followUser
					 * unfollowUser
					 * followPlaylist
					 * unfollowPlaylist
					 * getCategoryPlaylists
					 * getFeaturedPlaylists
					 * getGenreSeeds
					 * getRecommendations
					 * updateUserPlaylist
					 * getUserPlaylistTracks
					 * myAlbumsContains
					 * myTracksContains
					 * reorderUserPlaylistTracks
					 * replaceUserPlaylistTracks
					 * search
					 * userFollowsPlaylist
					 */

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