#include <gtest/gtest.h>
#include <Net/URI.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(URI, Parse)
	{
		{
			Net::URI uri("http://example.com");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("", uri.getUser());
			ASSERT_EQ("", uri.getPassword());
			ASSERT_EQ(-1, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/", uri.getPath());
			ASSERT_EQ("", uri.getQuery());
			ASSERT_EQ("", uri.getFragment());
			ASSERT_EQ("http://example.com/", uri.str());
		}
		{
			Net::URI uri("http://example.com/test.html");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("", uri.getUser());
			ASSERT_EQ("", uri.getPassword());
			ASSERT_EQ(-1, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/test.html", uri.getPath());
			ASSERT_EQ("", uri.getQuery());
			ASSERT_EQ("", uri.getFragment());
			ASSERT_EQ("http://example.com/test.html", uri.str());
		}
		{
			Net::URI uri("http://example.com/test.html?test");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("", uri.getUser());
			ASSERT_EQ("", uri.getPassword());
			ASSERT_EQ(-1, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/test.html", uri.getPath());
			ASSERT_EQ("?test", uri.getQuery());
			ASSERT_EQ("", uri.getFragment());
			ASSERT_EQ("http://example.com/test.html?test", uri.str());
		}
		{
			Net::URI uri("http://example.com/test.html#anker");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("", uri.getUser());
			ASSERT_EQ("", uri.getPassword());
			ASSERT_EQ(-1, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/test.html", uri.getPath());
			ASSERT_EQ("", uri.getQuery());
			ASSERT_EQ("#anker", uri.getFragment());
			ASSERT_EQ("http://example.com/test.html#anker", uri.str());
		}
		{
			Net::URI uri("http://test@example.com/test.html");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("test", uri.getUser());
			ASSERT_EQ("", uri.getPassword());
			ASSERT_EQ(-1, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/test.html", uri.getPath());
			ASSERT_EQ("", uri.getQuery());
			ASSERT_EQ("", uri.getFragment());
			ASSERT_EQ("http://test@example.com/test.html", uri.str());
		}
		{
			Net::URI uri("http://test:pw@example.com/test.html");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("test", uri.getUser());
			ASSERT_EQ("pw", uri.getPassword());
			ASSERT_EQ(-1, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/test.html", uri.getPath());
			ASSERT_EQ("", uri.getQuery());
			ASSERT_EQ("", uri.getFragment());
			ASSERT_EQ("http://test:pw@example.com/test.html", uri.str());
		}
		{
			Net::URI uri("http://example.com:8080/test.html");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("", uri.getUser());
			ASSERT_EQ("", uri.getPassword());
			ASSERT_EQ(8080, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/test.html", uri.getPath());
			ASSERT_EQ("", uri.getQuery());
			ASSERT_EQ("", uri.getFragment());
			ASSERT_EQ("http://example.com:8080/test.html", uri.str());
		}
		{
			Net::URI uri("http://example.com:8080/leer%20zeichen/test.html");
			ASSERT_EQ("http", uri.getScheme());
			ASSERT_EQ("", uri.getUser());
			ASSERT_EQ("", uri.getPassword());
			ASSERT_EQ(8080, uri.getPort());
			ASSERT_EQ("example.com", uri.getHostname());
			ASSERT_EQ("/leer zeichen/test.html", uri.getPath());
			ASSERT_EQ("", uri.getQuery());
			ASSERT_EQ("", uri.getFragment());
			ASSERT_EQ("http://example.com:8080/leer%20zeichen/test.html", uri.str());
		}
	}

	TEST(URI, URLEncoding)
	{
		std::string url_encoded = "Hallo%2C%20wie%20gehts%20%3F";
		std::string plain = "Hallo, wie gehts ?";
		auto decoded = Net::URI::URLDecode(url_encoded);
		ASSERT_EQ(plain, decoded);
		auto encoded = Net::URI::URLEncode(decoded);
		ASSERT_EQ(url_encoded, encoded);
	}
}