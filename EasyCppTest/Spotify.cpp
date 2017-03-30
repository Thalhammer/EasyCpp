#include <gtest/gtest.h>
#include <Net/Services/Spotify/Client.h>
#include <Net/Services/Spotify/Authorization.h>

using namespace EasyCpp::Net::Services::Spotify;

namespace EasyCppTest
{
	const std::string refresh_token = "AQDolrLKpDtIRYLRqg8tkV3mnCODZswhPkar67ZpkhqxDMGCiKMcvrFrB4mApEaWwg-yStvB8ulR0zUT-Fb0ffXwFuAmSrqJ6yDYC_pUqrOa9M9V-KUA_VDWnS2hs6JgDHs";

	std::string GetAccessTokenUser()
	{
		static std::string access_token;
		if (access_token == "") {
			Authorization auth;
			auth.setClientID("7a99705fff7042e18c61d03b8335b28e");
			auth.setClientSecret("8491c78d36da48808e2b6493573363d8");
			auto n = auth.refreshToken(refresh_token);
			access_token = n.getAccessToken();
		}
		return access_token;
	}

	std::string GetAccessTokenService()
	{
		static std::string access_token;
		if (access_token == "") {
			Authorization auth;
			auth.setClientID("7a99705fff7042e18c61d03b8335b28e");
			auth.setClientSecret("8491c78d36da48808e2b6493573363d8");
			auto n = auth.requestToken();
			access_token = n.getAccessToken();
		}
		return access_token;
	}

	Client GetClientUserAuth()
	{
		Client sp;
		sp.setAccessToken(GetAccessTokenUser());
		return sp;
	}

	Client GetClientServiceAuth()
	{
		Client sp;
		sp.setAccessToken(GetAccessTokenService());
		return sp;
	}

	TEST(Spotify, Search)
	{
		auto sp = GetClientServiceAuth();
		auto res = sp.search("Sample");
	}

	TEST(Spotify, CurrentUser)
	{
		auto sp = GetClientUserAuth();
		auto user = sp.getCurrentUser();
		ASSERT_EQ("experimental_user", user.getID());
	}

	TEST(Spotify, GetUser)
	{
		auto sp = GetClientUserAuth();
		auto user = sp.getUser("experimental_user");
		ASSERT_EQ("experimental_user", user.getID());
	}

	TEST(Spotify, GetAlbum)
	{
		auto sp = GetClientServiceAuth();
		auto album = sp.getAlbum("0sNOF9WDwhWunNAHPD3Baj");
	}

	TEST(Spotify, GetAlbums)
	{
		auto sp = GetClientServiceAuth();
		auto album = sp.getAlbums({
			"41MnTivkwTO3UUJ8DrqEJJ",
			"6JWc4iAiJ9FjyK0B59ABb4",
			"6UXCm6bOO4gFlDQZV5yL37"
		});
	}

	TEST(Spotify, GetAlbumTracks)
	{
		auto sp = GetClientServiceAuth();
		auto tracks = sp.getAlbumTracks("0sNOF9WDwhWunNAHPD3Baj");
	}

	TEST(Spotify, GetAlbumRelinking)
	{
		auto sp = GetClientServiceAuth();
		sp.setMarket("DE");
		auto album = sp.getAlbum("0sNOF9WDwhWunNAHPD3Baj");
	}

	TEST(Spotify, GetArtist)
	{
		auto sp = GetClientServiceAuth();
		auto artist = sp.getArtist("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetArtists)
	{
		auto sp = GetClientServiceAuth();
		auto artists = sp.getArtists({
			"2CIMQHirSU0MQqyYHq0eOx",
			"57dN52uHvrHOxijzpIgu3E",
			"1vCWHaC5f2uS3yhpwWbIA6"
		});
	}

	TEST(Spotify, GetArtistsAlbums)
	{
		auto sp = GetClientServiceAuth();
		auto albums = sp.getArtistAlbums("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetArtistsTopTracks)
	{
		auto sp = GetClientServiceAuth();
		sp.setMarket("DE");
		auto tracks = sp.getArtistTopTracks("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetArtistsRelatedTracks)
	{
		auto sp = GetClientServiceAuth();
		auto artists = sp.getArtistRelatedArtists("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetTrack)
	{
		auto sp = GetClientServiceAuth();
		auto track = sp.getTrack("1zHlj4dQ8ZAtrayhuDDmkY");
	}

	TEST(Spotify, GetTracks)
	{
		auto sp = GetClientServiceAuth();
		auto tracks = sp.getTracks({
			"7ouMYWpwJ422jRcDASZB7P",
			"4VqPOruhp5EdPBeR92t6lQ",
			"2takcwOaAZWiXQijPHIx7B"
		});
	}

	TEST(Spotify, AddAlbum)
	{
		auto sp = GetClientUserAuth();
		sp.addMyAlbums("4g8AOBMFKeuthiXpj6Kxmr");
	}

	TEST(Spotify, CreatePlaylist)
	{
		auto sp = GetClientUserAuth();
		auto me = sp.getCurrentUser();
		sp.createUserPlaylist(me.getID(), "Testplaylist");
	}

	TEST(Spotify, GetAudioFeatures)
	{
		auto sp = GetClientUserAuth();
		auto features = sp.getAudioFeatures("7ouMYWpwJ422jRcDASZB7P");
	}

	TEST(Spotify, DISABLED_Authorize)
	{
		Authorization auth;
		auth.setClientID("7a99705fff7042e18c61d03b8335b28e");
		auth.setClientSecret("8491c78d36da48808e2b6493573363d8");
		auth.setRedirectURI("http://localhost/");
		auth.setScopes({
			"playlist-read-private",
			"playlist-read-collaborative",
			"playlist-modify-public",
			"playlist-modify-private",
			"user-follow-modify",
			"user-follow-read",
			"user-library-read",
			"user-library-modify",
			"user-read-private",
			"user-read-birthdate",
			"user-read-email",
			"user-top-read"
		});
		auth.setShowDialog(true);

		std::string url = auth.getAuthURL();


		std::string code = "AQCQ4QNTur80TYRj8GEMm-8Qg0m5MaQV6-I6tJDTVpsQy5Iq1U6-kaTXC-9Z3db5pta4miqhGTP-H1Y8h8GVvqIuqqvIv9oj7V9i9wvcyjbrlwp5NQ9AY866u7kk9a5VI5N1ZH-HZqEaOLkA_i7OBoIA4mKB1PlUjBdKov5M4k95W60iKDPV7RMoCnkdRzDYeh7bqf2vHF61xzt-p57t7ua0_72-eOkl7kxHv1Ie0i3XydfDb2IQ_ylWO3Thk1XArQXByxIH64hAzMFr3J-nMAwYo77OoWFvvcOJv_CREadBF5GbuiJGh_nl5ntS7f0qX8OS2CLUhms9LnYlNfjWPgzNoIBQ7pW5d2inf3Gpy07V7-LNwgxBDHQiFZplX7UeahZmJYOGfRKw36-eqKrs1QHQkR2wP413zGhAtVyrAN6DLjHfJPCFor7H16JZ-ZDK6KyAZX6RtstAQDhF_-mwQBtq_GVWHrfzw5ethBgWNC3wuJvYRLAqw0xEfnbRZrnq_J4";
		auto token = auth.requestToken(code);
		auto token2 = auth.refreshToken(token.getRefreshToken());
	}
}