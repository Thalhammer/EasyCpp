#pragma once
#include "../../../../DllExport.h"
#include "CVDSMModelResult.h"
#include "CVDescribeImageResult.h"
#include "CVTagImageResult.h"
#include "CVOCRResult.h"
#include "CVAnalyseResult.h"
#include <memory>

namespace EasyCpp
{
	class Bundle;
	namespace Net
	{
		class Curl;
		namespace Services
		{
			namespace Microsoft
			{
				namespace Cognitive
				{
					enum class CVAnalyse
					{
						Faces,
						ImageType,
						Color,
						Adult,
						Description,
						Tags,
						Categories
					};

					class DLL_EXPORT ComputerVision
					{
					public:
						ComputerVision();
						virtual ~ComputerVision();

						void setOcpApimSubscriptionKey(const std::string& key);
						const std::string& getOcpApimSubscriptionKey() const;

						CVDSMModelResult getDSMModels();
						CVDescribeImageResult describeImage(const std::string& url, size_t max_candidates=1);
						CVDescribeImageResult describeImage(const std::vector<uint8_t>& data, size_t max_candidates=1);
						std::string getThumbnail(const std::string& url, size_t width = 256, size_t height = 256, bool smart_cropping = true);
						std::string getThumbnail(const std::vector<uint8_t>& data, size_t width = 256, size_t height = 256, bool smart_cropping = true);
						CVTagImageResult tagImage(const std::string& url);
						CVTagImageResult tagImage(const std::vector<uint8_t>& data);
						// TODO: Recognise Models

						CVOCRResult ocr(const std::string& url, const std::string& lang="unk", bool detect_orientation=true);
						CVOCRResult ocr(const std::vector<uint8_t>& data, const std::string& lang = "unk", bool detect_orientation = true);
						CVAnalyseResult analyse(const std::string& url, const std::vector<CVAnalyse>& features = {}, const std::vector<std::string>& details = {});
						CVAnalyseResult analyse(const std::vector<uint8_t>& data, const std::vector<CVAnalyse>& features = {}, const std::vector<std::string>& details = {});
					private:
						std::string _ocp_api_key;

						std::unique_ptr<Curl> setupCurl(const std::string& url);

						Bundle sendGet(const std::string& url);
						Bundle sendPost(const std::string& url, const std::string& data, bool bin);
					};
				}
			}
		}
	}
}
