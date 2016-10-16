#include <gtest/gtest.h>
#include <Net/Services/Microsoft/Cognitive/ComputerVision.h>

using namespace EasyCpp;
using namespace EasyCpp::Net::Services::Microsoft::Cognitive;

namespace EasyCppTest
{
	std::string cv_key = "0401255a33224f248e513ce97957563d";

	TEST(MSCognitive, ComputerVision_GetModels)
	{
		ComputerVision cv;
		cv.setOcpApimSubscriptionKey(cv_key);
		auto models = cv.getDSMModels();
	}

	TEST(MSCognitive, ComputerVision_DescribeImage)
	{
		ComputerVision cv;
		cv.setOcpApimSubscriptionKey(cv_key);
		auto description = cv.describeImage("http://www.technocrazed.com/wp-content/uploads/2015/12/Tree-wallpaper-to-use-as-background-133.jpg", 10);
	}

	TEST(MSCognitive, ComputerVision_TagImage)
	{
		ComputerVision cv;
		cv.setOcpApimSubscriptionKey(cv_key);
		auto tags = cv.tagImage("http://www.technocrazed.com/wp-content/uploads/2015/12/Tree-wallpaper-to-use-as-background-133.jpg");
		ASSERT_NE(0, tags.getTags().size());
	}

	TEST(MSCognitive, ComputerVision_OCR)
	{
		ComputerVision cv;
		cv.setOcpApimSubscriptionKey(cv_key);
		auto ocr = cv.ocr("http://content.altfonts.com:88/img/M/Y/Myriad-Apple-TextA.png");
		ASSERT_EQ("ABCDEFGHIJKLM\nNOPQRSTUVWXYZ", ocr.getText());
	}
}