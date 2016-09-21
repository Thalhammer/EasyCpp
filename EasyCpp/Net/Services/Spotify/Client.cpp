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
					Bundle bundle = doGET("/me", true);

					User res;
					res.fromAnyValue(bundle);
					return res;
				}

				PublicUser Client::getUser(const std::string & name)
				{
					Bundle bundle = doGET("/users/" + name, false);

					PublicUser res;
					res.fromAnyValue(bundle);
					return res;
				}

				FullAlbum Client::getAlbum(const std::string & uri)
				{
					Bundle bundle = doGET("/albums/" + uri + (_market != "" ? ("?market=" + _market) : ""), false);

					FullAlbum res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullAlbum> Client::getAlbums(const std::vector<std::string>& uris)
				{
					std::string url = "/albums?ids=" + implode<std::string>(",", uris);
					if (_market != "") url += "&market=" + _market;

					Bundle bundle = doGET(url, false);

					return fromAnyArray<FullAlbum>(bundle.get<AnyArray>("albums"));
				}

				Paging<Track> Client::getAlbumTracks(const std::string & album_uri, int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset);
					if (_market != "") params += "&market=" + _market;
					Bundle bundle = doGET("/albums/" + album_uri + "/tracks" + params, false);

					Paging<Track> res;
					res.fromAnyValue(bundle);
					return res;
				}

				FullArtist Client::getArtist(const std::string & uri)
				{
					Bundle bundle = doGET("/artists/" + uri + (_market != "" ? ("?market=" + _market) : ""), false);

					FullArtist res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullArtist> Client::getArtists(const std::vector<std::string>& uris)
				{
					std::string url = "/artists?ids=" + implode<std::string>(",", uris);
					if (_market != "") url += "&market=" + _market;

					Bundle bundle = doGET(url, false);

					return fromAnyArray<FullArtist>(bundle.get<AnyArray>("artists"));
				}

				Paging<Album> Client::getArtistsAlbums(const std::string & uri, const std::vector<std::string>& album_type, int limit, int offset)
				{
					std::string params = "?limit=" + std::to_string(limit) + "&offset=" + std::to_string(offset) + "&album_type=" + implode<std::string>(",", album_type);
					if (_market != "") params += "&market=" + _market;
					Bundle bundle = doGET("/artists/" + uri + "/albums" + params, false);

					Paging<Album> res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullTrack> Client::getArtistsTopTracks(const std::string & uri)
				{
					if (_market == "")
						throw Exception(0, "Market required but not set");
					Bundle bundle = doGET("/artists/" + uri + "/top-tracks?country=" + _market, false);

					return fromAnyArray<FullTrack>(bundle.get<AnyArray>("tracks"));
				}

				std::vector<FullArtist> Client::getArtistsRelatedArtists(const std::string & uri)
				{
					Bundle bundle = doGET("/artists/" + uri + "/related-artists", false);

					return fromAnyArray<FullArtist>(bundle.get<AnyArray>("artists"));
				}

				FullTrack Client::getTrack(const std::string & uri)
				{
					Bundle bundle = doGET("/tracks/" + uri + (_market != "" ? ("?market=" + _market) : ""), false);

					FullTrack res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<FullTrack> Client::getTracks(const std::vector<std::string>& uris)
				{
					std::string url = "/tracks?ids=" + implode<std::string>(",", uris);
					if (_market != "") url += "&market=" + _market;

					Bundle bundle = doGET(url, false);

					return fromAnyArray<FullTrack>(bundle.get<AnyArray>("tracks"));
				}

				AudioFeatures Client::getAudioFeatures(const std::string & uri)
				{
					Bundle bundle = doGET("/audio-features/" + uri, true);

					AudioFeatures res;
					res.fromAnyValue(bundle);
					return res;
				}

				std::vector<AudioFeatures> Client::getAudioFeatures(const std::vector<std::string>& uris)
				{
					Bundle bundle = doGET("/audio-features?ids=" + implode<std::string>(",", uris), true);

					return fromAnyArray<AudioFeatures>(bundle.get<AnyArray>("audio_features"));
				}

				Bundle Client::doGET(const std::string & url, bool req_auth)
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
						if (req_auth)
							throw Exception(-1, "Authorization required but no token set.");
					}
					curl.perform();

					Bundle bundle = Serialize::JsonSerializer().deserialize(str).as<Bundle>();

					if (bundle.isSet("error"))
					{
						Bundle error = bundle.get<Bundle>("error");
						throw Exception(error.get<int>("status"), error.get<std::string>("message"));
					}

					return bundle;
				}
			}
		}
	}
}
