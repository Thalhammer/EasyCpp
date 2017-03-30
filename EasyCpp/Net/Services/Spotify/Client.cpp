#include "Client.h"
#include "../../Curl.h"
#include "../../../Bundle.h"
#include "../../../Serialize/JsonSerializer.h"
#include "../../../StringAlgorithm.h"
#include "Exception.h"
#include "User.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Client::Client()
				{
					_basepath = "https://api.spotify.com/v1";
				}

				Client::~Client()
				{
				}

				void Client::setAccessToken(const std::string & str)
				{
					_token = str;
				}

				void Client::setMarket(const std::string & str)
				{
					_market = str;
				}

				User Client::getCurrentUser()
				{
					Bundle bundle = doGET("/me");

					User res;
					res.fromAnyValue(bundle);
					return res;
				}

				PublicUser Client::getUser(const std::string & name)
				{
					Bundle bundle = doGET("/users/" + name);

					PublicUser res;
					res.fromAnyValue(bundle);
					return res;
				}

				FullAlbum Client::getAlbum(const std::string & uri)
				{
					Bundle bundle = doGET("/albums/" + uri + (_market != "" ? ("?market=" + _market) : ""));

					FullAlbum res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullAlbum> Client::getAlbums(const std::vector<std::string>& uris)
				{
					std::string url = "/albums?ids=" + implode<std::string>(",", uris);
					if (_market != "") url += "&market=" + _market;

					Bundle bundle = doGET(url);

					return fromAnyArray<FullAlbum>(bundle.get<AnyArray>("albums"));
				}

				Paging<Track> Client::getAlbumTracks(const std::string & album_uri, int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset);
					if (_market != "") params += "&market=" + _market;
					Bundle bundle = doGET("/albums/" + album_uri + "/tracks" + params);

					Paging<Track> res;
					res.fromAnyValue(bundle);
					return res;
				}

				FullArtist Client::getArtist(const std::string & uri)
				{
					Bundle bundle = doGET("/artists/" + uri + (_market != "" ? ("?market=" + _market) : ""));

					FullArtist res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullArtist> Client::getArtists(const std::vector<std::string>& uris)
				{
					std::string url = "/artists?ids=" + implode<std::string>(",", uris);
					if (_market != "") url += "&market=" + _market;

					Bundle bundle = doGET(url);

					return fromAnyArray<FullArtist>(bundle.get<AnyArray>("artists"));
				}

				Paging<Album> Client::getArtistAlbums(const std::string & uri, const std::vector<std::string>& album_type, int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&album_type=" + implode<std::string>(",", album_type);
					if (_market != "") params += "&market=" + _market;
					Bundle bundle = doGET("/artists/" + uri + "/albums" + params);

					Paging<Album> res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullTrack> Client::getArtistTopTracks(const std::string & uri)
				{
					if (_market == "")
						throw Exception(0, "Market required but not set");
					Bundle bundle = doGET("/artists/" + uri + "/top-tracks?country=" + _market);

					return fromAnyArray<FullTrack>(bundle.get<AnyArray>("tracks"));
				}

				std::vector<FullArtist> Client::getArtistRelatedArtists(const std::string & uri)
				{
					Bundle bundle = doGET("/artists/" + uri + "/related-artists");

					return fromAnyArray<FullArtist>(bundle.get<AnyArray>("artists"));
				}

				FullTrack Client::getTrack(const std::string & uri)
				{
					Bundle bundle = doGET("/tracks/" + uri + (_market != "" ? ("?market=" + _market) : ""));

					FullTrack res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullTrack> Client::getTracks(const std::vector<std::string>& uris)
				{
					std::string url = "/tracks?ids=" + implode<std::string>(",", uris);
					if (_market != "") url += "&market=" + _market;

					Bundle bundle = doGET(url);

					return fromAnyArray<FullTrack>(bundle.get<AnyArray>("tracks"));
				}

				AudioFeatures Client::getAudioFeatures(const std::string & uri)
				{
					Bundle bundle = doGET("/audio-features/" + uri);

					AudioFeatures res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<AudioFeatures> Client::getAudioFeatures(const std::vector<std::string>& uris)
				{
					Bundle bundle = doGET("/audio-features?ids=" + implode<std::string>(",", uris));

					return fromAnyArray<AudioFeatures>(bundle.get<AnyArray>("audio_features"));
				}

				Paging<Category> Client::getCategoriesList(const std::string & locale, int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					params += (_market != "" ? ("&country=" + _market) : "");
					params += (locale != "" ? ("&locale=" + locale) : "");

					Bundle bundle = doGET("/browse/categories" + params);

					Paging<Category> res("categories");
					res.fromAnyValue(bundle);
					return res;
				}

				Category Client::getCategory(const std::string & id, const std::string & locale)
				{
					std::string params;
					params += (_market != "" ? ("&country=" + _market) : "");
					params += (locale != "" ? ("&locale=" + locale) : "");
					if (!params.empty()) params[0] = '?';
					Bundle bundle = doGET("/browse/categories/" + id + params);

					Category res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<Playlist> Client::getCategoryPlaylists(const std::string & catid, int limit, int offset)
				{
					std::string params;
					params += "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					params += (_market != "" ? ("&country=" + _market) : "");
					Bundle bundle = doGET("/browse/categories/" + catid + params);

					Paging<Playlist> res;
					res.fromAnyValue(bundle.get<Bundle>("playlists"));
					return res;
				}

				Paging<Album> Client::getNewReleases(int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					params += (_market != "" ? ("&country=" + _market) : "");
					Bundle bundle = doGET("/browse/new-releases" + params);

					Paging<Album> res("albums");
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<SavedAlbum> Client::getMySavedAlbums(int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					params += (_market != "" ? ("&market=" + _market) : "");
					Bundle bundle = doGET("/me/albums" + params);

					Paging<SavedAlbum> res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<SavedTrack> Client::getMySavedTracks(int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					params += (_market != "" ? ("&market=" + _market) : "");
					Bundle bundle = doGET("/me/tracks" + params);

					Paging<SavedTrack> res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<FullTrack> Client::getMyTopTracks(int limit, int offset, TimeRange range)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					if (range == TimeRange::LONG) params += "&time_range=long_term";
					else if (range == TimeRange::MEDIUM) params += "&time_range=medium_term";
					else if (range == TimeRange::SHORT) params += "&time_range=short_term";

					Bundle bundle = doGET("/me/top/tracks" + params);

					Paging<FullTrack> res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<FullArtist> Client::getMyTopArtists(int limit, int offset, TimeRange range)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					if (range == TimeRange::LONG) params += "&time_range=long_term";
					else if (range == TimeRange::MEDIUM) params += "&time_range=medium_term";
					else if (range == TimeRange::SHORT) params += "&time_range=short_term";

					Bundle bundle = doGET("/me/top/artists" + params);

					Paging<FullArtist> res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<FullArtist> Client::getUserFollowedArtists(int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					params += "&type=artist";

					Bundle bundle = doGET("/me/following" + params);

					Paging<FullArtist> res("artists");
					res.fromAnyValue(bundle);
					return res;
				}

				FullPlaylist Client::getUserPlaylist(const std::string & uid, const std::string & pid)
				{
					Bundle bundle = doGET("/users/" + uid + "/playlists/" + pid + (_market != "" ? ("?market=" + _market) : ""));

					FullPlaylist res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<Playlist> Client::getUserPlaylists(const std::string & uid, int limit, int offset)
				{
					Bundle bundle = doGET("/users/" + uid + "/playlists");

					Paging<Playlist> res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<Playlist> Client::getMyPlaylists(int limit, int offset)
				{
					Bundle bundle = doGET("/me/playlists");

					Paging<Playlist> res;
					res.fromAnyValue(bundle);
					return res;
				}

				Paging<PlaylistTrack> Client::getUserPlaylistTracks(const std::string & uid, const std::string & pid, int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					if (!_market.empty()) params += "&market=" + _market;
					Bundle bundle = doGET("/users/" + uid + "/playlists/" + pid + "/tracks" + params);

					Paging<PlaylistTrack> res;
					res.fromAnyValue(bundle);
					return res;
				}

				FeaturedPlaylists Client::getFeaturedPlaylists(const std::string & locale, int limit, int offset, const std::string & timestamp)
				{
					std::string params = "?limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					params += "&locale=" + locale;
					if (_market != "") params += "&country=" + _market;
					if (timestamp != "") params += "&timestamp=" + timestamp;
					Bundle bundle = doGET("/browse/featured-playlists" + params);

					FeaturedPlaylists res;
					res.fromAnyValue(bundle);
					return res;
				}

				void Client::addMyAlbums(const std::vector<std::string>& ids)
				{
					this->doPUT("/me/albums", toAnyArray(ids));
				}

				void Client::addMyAlbums(const std::string & id)
				{
					this->addMyAlbums(std::vector<std::string>({ id }));
				}

				void Client::deleteMyAlbums(const std::vector<std::string>& ids)
				{
					this->doDELETE("/me/albums", toAnyArray(ids));
				}

				void Client::deleteMyAlbums(const std::string & id)
				{
					this->deleteMyAlbums(std::vector<std::string>({ id }));
				}

				void Client::addMyTracks(const std::vector<std::string>& ids)
				{
					this->doPUT("/me/tracks", toAnyArray(ids));
				}

				void Client::addMyTracks(const std::string & id)
				{
					this->addMyTracks(std::vector<std::string>({ id }));
				}

				void Client::deleteMyTracks(const std::vector<std::string>& ids)
				{
					this->doDELETE("/me/tracks", toAnyArray(ids));
				}

				void Client::deleteMyTracks(const std::string & id)
				{
					this->deleteMyTracks(std::vector<std::string>({ id }));
				}

				void Client::addUserPlaylistTracks(const std::string & uid, const std::string & pid, const std::vector<std::string>& ids, int position)
				{
					std::string params = (position != -1 ? ("?position=" + std::to_string(position)) : "");
					this->doPOST("/users/" + uid + "/playlists/" + pid + "/tracks" + params, Bundle({
						{"uris", toAnyArray(ids)}
					}));
				}

				std::string Client::deleteUserPlaylistTracks(const std::string & uid, const std::string & pid, const std::vector<std::string>& ids)
				{
					AnyArray array;
					for (auto& e : ids) {
						array.push_back(Bundle({
							{"uri", e}
						}));
					}
					auto res = this->doDELETE("/users/" + uid + "/playlists/" + pid + "/tracks", Bundle({
						{ "tracks", array }
					}));

					return res.as<Bundle>().get<std::string>("snapshot_id");
				}

				std::string Client::deleteUserPlaylistTracks(const std::string& uid, const std::string& pid, const std::vector<std::pair<std::string, std::vector<size_t>>>& uris, const std::string& snapshot)
				{
					AnyArray array;
					for (auto& e : uris) {
						array.push_back(Bundle({
							{ "uri", e.first },
							{ "positions", toAnyArray(e.second)}
						}));
					}
					Bundle data({
						{ "tracks", array }
					});
					if (snapshot != "") data.set("snapshot_id", snapshot);
					auto res = this->doDELETE("/users/" + uid + "/playlists/" + pid + "/tracks", data);

					return res.as<Bundle>().get<std::string>("snapshot_id");
				}

				std::string Client::deleteUserPlaylistTracks(const std::string & uid, const std::string & pid, const std::vector<size_t>& positions, const std::string & snapshot)
				{
					auto res = this->doDELETE("/users/" + uid + "/playlists/" + pid + "/tracks", Bundle({
						{"positions", toAnyArray(positions)},
						{"snapshot_id", snapshot}
					}));

					return res.as<Bundle>().get<std::string>("snapshot_id");
				}

				std::string Client::reorderUserPlaylistTracks(const std::string & uid, const std::string & pid, size_t range_start, size_t insert_before, size_t range_length, const std::string & snapshot)
				{
					Bundle data({
						{"range_start", range_start},
						{"range_length", range_length},
						{"insert_before", insert_before}
					});
					if (snapshot != "") data.set("snapshot", snapshot);
					auto res = this->doPUT("/users/" + uid + "/playlists/" + pid + "/tracks", data);
					return res.as<Bundle>().get<std::string>("snapshot_id");
				}

				void Client::replaceUserPlaylistTracks(const std::string & uid, const std::string & pid, const std::vector<std::string>& uris)
				{
					this->doPUT("/users/" + uid + "/playlists/" + pid + "/tracks", Bundle({
						{"uris", toAnyArray(uris)}
					}));
				}

				FullPlaylist Client::createUserPlaylist(const std::string & uid, const std::string & name, bool pub)
				{
					auto bundle = this->doPOST("/users/" + uid + "/playlists", Bundle({
						{ "name", name },
						{ "public", pub }
					}));

					FullPlaylist res;
					res.fromAnyValue(bundle);
					return res;
				}

				void Client::updateUserPlaylist(const std::string & uid, const std::string & pid, const std::string & new_name, bool new_public)
				{
					this->doPUT("/users/" + uid + "/playlists/" + pid, Bundle({
						{"name", new_name},
						{"public", new_public}
					}));
				}

				void Client::followArtist(const std::vector<std::string>& ids)
				{
					this->doPUT("/me/following?type=artist", Bundle({
						{"ids", toAnyArray(ids)}
					}));
				}

				void Client::followArtist(const std::string & id)
				{
					this->followArtist(std::vector<std::string>({ id }));
				}

				void Client::followUser(const std::vector<std::string>& ids)
				{
					this->doPUT("/me/following?type=user", Bundle({
						{ "ids", toAnyArray(ids) }
					}));
				}

				void Client::followUser(const std::string & id)
				{
					this->followUser(std::vector<std::string>({ id }));
				}

				void Client::followPlaylist(const std::string & puid, const std::string & pid, bool pub)
				{
					this->doPUT("/users/" + puid + "/playlists/" + pid + "/followers", Bundle({
						{ "public", pub }
					}));
				}

				void Client::unfollowArtist(const std::vector<std::string>& ids)
				{
					this->doDELETE("/me/following?type=artist", Bundle({
						{"ids", toAnyArray(ids)}
					}));
				}

				void Client::unfollowArtist(const std::string & id)
				{
					this->unfollowArtist(std::vector<std::string>({ id }));
				}

				void Client::unfollowUser(const std::vector<std::string>& ids)
				{
					this->doDELETE("/me/following?type=user", Bundle({
						{ "ids", toAnyArray(ids) }
					}));
				}

				void Client::unfollowUser(const std::string & id)
				{
					this->unfollowUser(std::vector<std::string>({ id }));
				}

				void Client::unfollowPlaylist(const std::string & puid, const std::string & pid)
				{
					this->doDELETE("/users/" + puid + "/playlists/" + pid + "/followers", AnyValue());
				}

				std::vector<bool> Client::userFollowsPlaylist(const std::string & puid, const std::string & pid, const std::vector<std::string>& ids)
				{
					auto info = this->doGET("/users/" + puid + "/playlists/" + pid + "/followers/contains?ids=" + implode<std::string>(",", ids));
					return fromAnyArray<bool>(info);
				}

				bool Client::userFollowsPlaylist(const std::string & puid, const std::string & pid, const std::string & id)
				{
					return this->userFollowsPlaylist(puid, pid, std::vector<std::string>({ id }))[0];
				}

				std::vector<bool> Client::userFollowsArtist(const std::vector<std::string>& ids)
				{
					auto info = this->doGET("/me/following/contains?type=artist&ids=" + implode<std::string>(",", ids));
					return fromAnyArray<bool>(info);
				}

				bool Client::userFollowsArtist(const std::string & id)
				{
					return this->userFollowsArtist(std::vector<std::string>({ id }))[0];
				}

				std::vector<bool> Client::userFollowsUser(const std::vector<std::string>& ids)
				{
					auto info = this->doGET("/me/following/contains?type=user&ids=" + implode<std::string>(",", ids));
					return fromAnyArray<bool>(info);
				}

				bool Client::userFollowsUser(const std::string & id)
				{
					return this->userFollowsUser(std::vector<std::string>({ id }))[0];
				}

				std::vector<bool> Client::myTracksContains(const std::vector<std::string>& ids)
				{
					auto info = this->doGET("/me/tracks/contains?ids=" + implode<std::string>(",", ids));
					return fromAnyArray<bool>(info);
				}

				bool Client::myTracksContains(const std::string & id)
				{
					return this->myTracksContains(std::vector<std::string>({ id }))[0];
				}

				std::vector<bool> Client::myAlbumsContains(const std::vector<std::string>& ids)
				{
					auto info = this->doGET("/me/albums/contains?ids=" + implode<std::string>(",", ids));
					return fromAnyArray<bool>(info);
				}

				bool Client::myAlbumsContains(const std::string & id)
				{
					return this->myAlbumsContains(std::vector<std::string>({ id }))[0];
				}

				RecommendationResponse Client::getRecommendations(const std::vector<std::string>& seed_artists, const std::vector<std::string>& seed_tracks, const std::vector<std::string>& seed_genres, const std::map<std::string, std::string>& max, const std::map<std::string, std::string>& min, const std::map<std::string, std::string>& target, size_t limit)
				{
					std::string params;
					params += "?seed_artists=" + implode<std::string>(",", seed_artists);
					params += "&seed_tracks=" + implode<std::string>(",", seed_tracks);
					params += "&seed_genres=" + implode<std::string>(",", seed_genres);
					for (auto& e : max) {
						params += "&max_" + e.first + "=" + e.second;
					}
					for (auto& e : min) {
						params += "&min_" + e.first + "=" + e.second;
					}
					for (auto& e : target) {
						params += "&target_" + e.first + "=" + e.second;
					}
					if (_market != "") params += "&market=" + _market;
					params += "&limit=" + std::to_string(limit);

					auto info = this->doGET("/recommendations" + params);

					RecommendationResponse res;
					res.fromAnyValue(info);
					return res;
				}

				std::vector<std::string> Client::getGenreSeeds()
				{
					auto info = this->doGET("/recommendations/available-genre-seeds");

					return fromAnyArray<std::string>(info.as<Bundle>().get<AnyArray>("genres"));
				}

				SearchResult Client::search(const std::string & q, const std::vector<std::string>& types, size_t limit, size_t offset)
				{
					std::string params = "?q=" + URI::URLEncode(q);
					params += "&type=" + implode<std::string>(",", types);
					params += "&limit=" + std::to_string(limit);
					params += "&offset=" + std::to_string(offset);
					if (!_market.empty()) params += "&market=" + _market;

					auto info = this->doGET("/search" + params);

					SearchResult res;
					res.fromAnyValue(info);
					return res;
				}

				AnyValue Client::doGET(const std::string & url)
				{
					std::string str;
					Curl curl;
					curl.setURL(_basepath + url);
					curl.setOutputString(str);
					curl.setSSLCABundle("curl-ca-bundle.crt");
					if (_token != "")
					{
						curl.setHeaders({
							{ "Authorization", "Bearer " + _token }
						});
					}
					else {
						throw Exception(-1, "Authorization required but no token set.");
					}
					curl.perform();

					AnyValue res = Serialize::JsonSerializer().deserialize(str);

					if (res.isType<Bundle>())
					{
						Bundle bundle = res.as<Bundle>();
						if (bundle.isSet("error")) {
							Bundle error = bundle.get<Bundle>("error");
							throw Exception(error.get<int>("status"), error.get<std::string>("message"));
						}
					}

					return res;
				}

				AnyValue Client::doPUT(const std::string & url, AnyValue body)
				{
					std::string input;
					if (!body.isType<nullptr_t>()) {
						input = Serialize::JsonSerializer().serialize(body);
					}

					std::string str;
					Curl curl;
					curl.setURL(_basepath + url);
					curl.setPOST(true);
					curl.setOutputString(str);
					curl.setInputString(input);
					curl.setSSLCABundle("curl-ca-bundle.crt");
					curl.setHeaders({
						{ "Authorization", "Bearer " + _token },
						{ "Content-Type", "application/json"}
					});
					curl.setCustomRequest("PUT");
					curl.perform();

					if (curl.getResponseCode() != 200 && curl.getResponseCode() != 201 && curl.getResponseCode() != 204)
					{
						if (str != "") {
							Bundle error = Serialize::JsonSerializer().deserialize(str).as<Bundle>().get<Bundle>("error");
							throw Exception(error.get<int>("status"), error.get<std::string>("message"));
						}
						throw Exception(curl.getResponseCode(), "HTTP Error");
					}
					if(str != "")
						return Serialize::JsonSerializer().deserialize(str);
					else return AnyValue();
				}

				AnyValue Client::doPOST(const std::string & url, AnyValue body)
				{
					std::string input;
					if (!body.isType<nullptr_t>()) {
						input = Serialize::JsonSerializer().serialize(body);
					}

					std::string str;
					Curl curl;
					curl.setURL(_basepath + url);
					curl.setPOST(true);
					curl.setOutputString(str);
					curl.setInputString(input);
					curl.setSSLCABundle("curl-ca-bundle.crt");
					curl.setHeaders({
						{ "Authorization", "Bearer " + _token },
						{ "Content-Type", "application/json" }
					});
					curl.perform();

					if (curl.getResponseCode() != 200 && curl.getResponseCode() != 201 && curl.getResponseCode() != 204)
					{
						if (str != "") {
							Bundle error = Serialize::JsonSerializer().deserialize(str).as<Bundle>().get<Bundle>("error");
							throw Exception(error.get<int>("status"), error.get<std::string>("message"));
						}
						throw Exception(curl.getResponseCode(), "HTTP Error");
					}
					if (str != "")
						return Serialize::JsonSerializer().deserialize(str);
					else return AnyValue();
				}

				AnyValue Client::doDELETE(const std::string & url, AnyValue body)
				{
					std::string input;
					if (!body.isType<nullptr_t>()) {
						input = Serialize::JsonSerializer().serialize(body);
					}

					std::string str;
					Curl curl;
					curl.setURL(_basepath + url);
					curl.setPOST(true);
					curl.setOutputString(str);
					curl.setInputString(input);
					curl.setSSLCABundle("curl-ca-bundle.crt");
					curl.setHeaders({
						{ "Authorization", "Bearer " + _token },
						{ "Content-Type", "application/json" }
					});
					curl.setCustomRequest("DELETE");
					curl.perform();

					if (curl.getResponseCode() != 200 && curl.getResponseCode() != 201 && curl.getResponseCode() != 204)
					{
						if (str != "") {
							Bundle error = Serialize::JsonSerializer().deserialize(str).as<Bundle>().get<Bundle>("error");
							throw Exception(error.get<int>("status"), error.get<std::string>("message"));
						}
						throw Exception(curl.getResponseCode(), "HTTP Error");
					}
					if (str != "")
						return Serialize::JsonSerializer().deserialize(str);
					else return AnyValue();
				}
			}
		}
	}
}
