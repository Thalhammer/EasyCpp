#include <gtest/gtest.h>
#include <Net/Services/Spotify/Client.h>
#include <Net/Services/Spotify/Authorization.h>

using namespace EasyCpp::Net::Services::Spotify;

namespace EasyCppTest
{
	const std::string access_token =
		"BQAYRaKBADL6fo6pOmH9qkxzapedlvTAHWk9FO8jgiJQaxdbFGeH5WgvdJ_1zZIQB8pmpOfgZitThckYDNsfXoHmW641XT0W9J-IdZ5mlqY3EjWxPJIw_zp8qAHNLR19vy7S897HW2Y8lvInAEOJ3-vnfBnosn10JVgXURv_6QUxrpoPczNTXs9vtiRKGvugM3qDEuvj9LCz3R-WGDsu-cCHSLk1GPXGbC-YS7inWdhPUfZXF9DcgW-oPaCvmZf6A5LI6tnYtK6g9CZ3w7tIyVZIxlavQ_xA9LvrAZzk06lGG-7NTuoh5oAEJRpB";

	TEST(Spotify, DISABLED_CurrentUser)
	{
		Client sp;
		sp.setAccessToken(access_token);
		auto user = sp.getCurrentUser();
		ASSERT_EQ("th-dev", user.getID());
	}

	TEST(Spotify, GetUser)
	{
		Client sp;
		auto user = sp.getUser("th-dev");
		ASSERT_EQ("th-dev", user.getID());
	}

	TEST(Spotify, GetAlbum)
	{
		Client sp;
		auto album = sp.getAlbum("0sNOF9WDwhWunNAHPD3Baj");
	}

	TEST(Spotify, GetAlbums)
	{
		Client sp;
		auto album = sp.getAlbums({
			"41MnTivkwTO3UUJ8DrqEJJ",
			"6JWc4iAiJ9FjyK0B59ABb4",
			"6UXCm6bOO4gFlDQZV5yL37"
		});
	}

	TEST(Spotify, GetAlbumTracks)
	{
		Client sp;
		auto tracks = sp.getAlbumTracks("0sNOF9WDwhWunNAHPD3Baj");
	}

	TEST(Spotify, GetAlbumRelinking)
	{
		Client sp;
		sp.setMarket("DE");
		auto album = sp.getAlbum("0sNOF9WDwhWunNAHPD3Baj");
	}

	TEST(Spotify, GetArtist)
	{
		Client sp;
		auto artist = sp.getArtist("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetArtists)
	{
		Client sp;
		auto artists = sp.getArtists({
			"2CIMQHirSU0MQqyYHq0eOx",
			"57dN52uHvrHOxijzpIgu3E",
			"1vCWHaC5f2uS3yhpwWbIA6"
		});
	}

	TEST(Spotify, GetArtistsAlbums)
	{
		Client sp;
		auto albums = sp.getArtistAlbums("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetArtistsTopTracks)
	{
		Client sp;
		sp.setMarket("DE");
		auto tracks = sp.getArtistTopTracks("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetArtistsRelatedTracks)
	{
		Client sp;
		auto artists = sp.getArtistRelatedArtists("0OdUWJ0sBjDrqHygGUXeCF");
	}

	TEST(Spotify, GetTrack)
	{
		Client sp;
		auto track = sp.getTrack("1zHlj4dQ8ZAtrayhuDDmkY");
	}

	TEST(Spotify, GetTracks)
	{
		Client sp;
		auto tracks = sp.getTracks({
			"7ouMYWpwJ422jRcDASZB7P",
			"4VqPOruhp5EdPBeR92t6lQ",
			"2takcwOaAZWiXQijPHIx7B"
		});
	}

	TEST(Spotify, DISABLED_GetAudioFeatures)
	{
		Client sp;
		sp.setAccessToken(access_token);
		auto features = sp.getAudioFeatures("7ouMYWpwJ422jRcDASZB7P");
	}

	TEST(Spotify, DISABLED_Authorize)
	{
		Authorization auth;
		auth.setClientID("7a99705fff7042e18c61d03b8335b28e");
		auth.setClientSecret("8491c78d36da48808e2b6493573363d8");
		auth.setRedirectURI("http://localhost/");
		auth.setScopes({
			"user-read-private",
			"user-read-birthdate",
			"user-read-email"
		});
		auth.setShowDialog(true);

		std::string url = auth.getAuthURL();


		std::string code = "AQCh6fYdZRTD9GS0E1vcvBiN0C99BDQ1u7iNEVryzPk_EfoL93NLohamorGKt6YAmUFHNGQMTvk4WKRUu3dBFtyKYshrHWVEBKgQ_nDz3IXNFnU0B2pMItxiGd8xN6D15U4TBEAYzyy9gkVkYEBPq_XScgTwAtDHvNx7x1HUcxctrbv9eBYmJVYZ1SgFtjlzqK6hqjU51TKVFhw-apTcUoTFceBs-B_zPSeVviNulalbEfdlq8XUogfe_9Z4JHNK";

		auto token = auth.requestToken(code);
		auto token2 = auth.refreshToken(token.getRefreshToken());
	}
}