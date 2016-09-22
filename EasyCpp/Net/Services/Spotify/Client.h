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
#include "FeaturedPlaylists.h"

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
					Paging<Playlist> getCategoryPlaylists(const std::string& catid, int limit = 20, int offset = 0);
					
					Paging<Album> getNewReleases(int limit = 20, int offset = 0);

					Paging<SavedAlbum> getMySavedAlbums(int limit = 20, int offset = 0);
					Paging<SavedTrack> getMySavedTracks(int limit = 20, int offset = 0);
					Paging<FullTrack> getMyTopTracks(int limit = 20, int offset = 0, TimeRange range = TimeRange::MEDIUM);
					Paging<FullArtist> getMyTopArtists(int limit = 20, int offset = 0, TimeRange range = TimeRange::MEDIUM);
					Paging<FullArtist> getUserFollowedArtists(int limit = 20, int offset = 0);

					FullPlaylist getUserPlaylist(const std::string& uid, const std::string& pid);
					Paging<Playlist> getUserPlaylists(const std::string& uid, int limit = 20, int offset = 0);
					Paging<Playlist> getMyPlaylists(int limit = 20, int offset = 0);
					Paging<PlaylistTrack> getUserPlaylistTracks(const std::string& uid, const std::string& pid, int limit = 20, int offset = 0);

					FeaturedPlaylists getFeaturedPlaylists(const std::string& locale, int limit = 20, int offset = 0, const std::string& timestamp="");
					
					void addMyAlbums(const std::vector<std::string>& ids);
					void addMyAlbums(const std::string& id);
					void deleteMyAlbums(const std::vector<std::string>& ids);
					void deleteMyAlbums(const std::string& id);
					void addMyTracks(const std::vector<std::string>& ids);
					void addMyTracks(const std::string& id);
					void deleteMyTracks(const std::vector<std::string>& ids);
					void deleteMyTracks(const std::string& id);

					void addUserPlaylistTracks(const std::string& uid, const std::string& pid, const std::vector<std::string>& ids, int position = -1);
					std::string deleteUserPlaylistTracks(const std::string& uid, const std::string& pid, const std::vector<std::string>& uris);
					std::string deleteUserPlaylistTracks(const std::string& uid, const std::string& pid, const std::vector<std::pair<std::string, std::vector<size_t>>>& uris, const std::string& snapshot = "");
					std::string deleteUserPlaylistTracks(const std::string& uid, const std::string& pid, const std::vector<size_t>& positions, const std::string& snapshot);
					std::string reorderUserPlaylistTracks(const std::string& uid, const std::string& pid, size_t range_start, size_t insert_before, size_t range_length = 1, const std::string& snapshot = "");
					void replaceUserPlaylistTracks(const std::string& uid, const std::string& pid, const std::vector<std::string>& uris);
					FullPlaylist createUserPlaylist(const std::string& uid, const std::string& name, bool pub = false);
					void updateUserPlaylist(const std::string& uid, const std::string& pid, const std::string& new_name, bool new_public);

					void followArtist(const std::vector<std::string>& ids);
					void followArtist(const std::string& id);
					void followUser(const std::vector<std::string>& ids);
					void followUser(const std::string& id);
					void followPlaylist(const std::string& puid, const std::string& pid, bool pub = false);
					void unfollowArtist(const std::vector<std::string>& ids);
					void unfollowArtist(const std::string& id);
					void unfollowUser(const std::vector<std::string>& ids);
					void unfollowUser(const std::string& id);
					void unfollowPlaylist(const std::string& puid, const std::string& pid);

					std::vector<bool> userFollowsPlaylist(const std::string& puid, const std::string& pid, const std::vector<std::string>& ids);
					bool userFollowsPlaylist(const std::string& puid, const std::string& pid, const std::string& id);
					std::vector<bool> userFollowsArtist(const std::vector<std::string>& ids);
					bool userFollowsArtist(const std::string& id);
					std::vector<bool> userFollowsUser(const std::vector<std::string>& ids);
					bool userFollowsUser(const std::string& id);
					std::vector<bool> myTracksContains(const std::vector<std::string>& ids);
					bool myTracksContains(const std::string& id);
					std::vector<bool> myAlbumsContains(const std::vector<std::string>& ids);
					bool myAlbumsContains(const std::string& id);
					/* TODO:
					 * getGenreSeeds
					 * getRecommendations
					 * search
					 */

				private:
					std::string _token;
					std::string _basepath;

					std::string _market;

					AnyValue doGET(const std::string& url, bool req_auth = true);
					AnyValue doPUT(const std::string& url, AnyValue body);
					AnyValue doPOST(const std::string& url, AnyValue body);
					AnyValue doDELETE(const std::string& url, AnyValue body);
				};
			}
		}
	}
}