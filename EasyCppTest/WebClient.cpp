#include <gtest/gtest.h>
#include <Net/WebClient.h>

using namespace EasyCpp;

namespace EasyCppTest
{
	TEST(WebClient, Download)
	{
		Net::WebClient wc;
		wc.setBaseAddress(Net::URI("http://google.de"));
		std::string result = wc.Download("/");
	}
}