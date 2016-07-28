#include <gtest/gtest.h>
#include <Net/Curl.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(CURL, SimpleGet)
	{
		std::string result;
		Net::Curl curl;
		curl.setURL("http://google.de");
		curl.setOutputString(result);
		curl.setUserAgent("libcurl-agent/1.0");
		std::string debug;
		curl.setDebugFunction([&debug](auto type, auto str, auto len) {
			if (type == Net::Curl::InfoType::Text)
			{
				debug += std::string(str, str + len);
			}
		});
		curl.setVerbose(true);
		curl.perform();
		//ASSERT_NE(0, result.length());
	}
	TEST(CURL, ConnectFailed)
	{
		Net::Curl curl;
		curl.setURL("http://127.0.0.2/test.html");
		ASSERT_THROW({
			curl.perform();
		}, std::runtime_error);
	}
	TEST(CURL, GetInfo)
	{
		Net::Curl curl;
		std::string debug;
		curl.setDebugFunction([&debug](auto type, auto str, auto len) {
			if (type == Net::Curl::InfoType::Text)
			{
				debug += std::string(str, str + len);
			}
		});
		curl.setVerbose(true);
		//curl.setSSLVerifyPeer(false);
		curl.setSSLCertInfo(true);
		curl.setSSLCABundle("curl-ca-bundle.crt");
		curl.setURL("https://google.com");
		curl.perform();
		auto info = curl.getCertInfo();
		double speed = curl.getSpeedDownload();
		double size = curl.getSizeDownload();
	}
	TEST(CURL, SpotifyTest)
	{
		std::string api_base = "https://api.spotify.com/v1";
		std::string result;
		{
			Net::Curl curl;
			curl.setURL(api_base + "/tracks/5c8XfmZCHnEugUgHdiXna8");
			curl.setSSLCABundle("curl-ca-bundle.crt");
			curl.setOutputString(result);
			curl.perform();
		}

	}
	TEST(CURL, SimplePOST)
	{
		std::string content = "Hallo Welt";
		Net::Curl curl;
		std::string debug;
		curl.setDebugFunction([&debug](auto type, auto str, auto len) {
			if (type == Net::Curl::InfoType::Text)
			{
				debug += std::string(str, str + len);
			}
		});
		curl.setVerbose(true);
		curl.setURL("http://requestb.in/1ly0noe1");
		curl.setPOST(true);
		curl.setInputString(content);
		//curl.setPOSTFields(content);
		curl.perform();
	}
}