#include "ComputerVision.h"
#include "../../../Curl.h"
#include "../../../../Serialize/JsonSerializer.h"
#include "ApiException.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Microsoft
			{
				namespace Cognitive
				{
					ComputerVision::ComputerVision()
					{
					}

					ComputerVision::~ComputerVision()
					{
					}

					void ComputerVision::setOcpApimSubscriptionKey(const std::string & key)
					{
						_ocp_api_key = key;
					}

					const std::string & ComputerVision::getOcpApimSubscriptionKey() const
					{
						return _ocp_api_key;
					}

					CVDSMModelResult ComputerVision::getDSMModels()
					{
						CVDSMModelResult res;
						res.fromAnyValue(sendGet("/models"));
						return res;
					}

					CVDescribeImageResult ComputerVision::describeImage(const std::string & url, size_t max_candidates)
					{
						std::string json = Serialize::JsonSerializer().serialize(Bundle({ { "url", url } }));

						CVDescribeImageResult res;
						res.fromAnyValue(sendPost("/describe?maxCandidates=" + std::to_string(max_candidates), json, false));
						return res;
					}

					CVDescribeImageResult ComputerVision::describeImage(const std::vector<uint8_t>& data, size_t max_candidates)
					{
						std::string image(data.cbegin(), data.cend());
						CVDescribeImageResult res;
						res.fromAnyValue(sendPost("/describe?maxCandidates=" + std::to_string(max_candidates), image, true));
						return res;
					}

					std::string ComputerVision::getThumbnail(const std::string & url, size_t width, size_t height, bool smart_cropping)
					{
						auto curl = setupCurl("/generateThumbnail?width=" + std::to_string(width) + "&height=" + std::to_string(height) + "&smartCropping=" + (smart_cropping?"true":"false"));
						curl->setHeaders({
							{ "Ocp-Apim-Subscription-Key", _ocp_api_key },
							{ "Content-Type", "application/json" }
						});
						std::string res;
						std::string json = Serialize::JsonSerializer().serialize(Bundle({ { "url", url } }));
						curl->setOutputString(res);
						curl->setInputString(json);
						curl->setPOST(true);
						curl->perform();
						if (curl->getResponseCode() != 200)
						{
							Bundle bres = Serialize::JsonSerializer().deserialize(res).as<Bundle>();
							throw ApiException(bres.get<std::string>("code"), bres.get<std::string>("message"), bres.get<std::string>("requestId"));
						}
						return res;
					}

					std::string ComputerVision::getThumbnail(const std::vector<uint8_t>& data, size_t width, size_t height, bool smart_cropping)
					{
						auto curl = setupCurl("/generateThumbnail?width=" + std::to_string(width) + "&height=" + std::to_string(height) + "&smartCropping=" + (smart_cropping ? "true" : "false"));
						curl->setHeaders({
							{ "Ocp-Apim-Subscription-Key", _ocp_api_key },
							{ "Content-Type", "application/octet-stream" }
						});
						std::string res;
						std::string input(data.cbegin(), data.cend());
						curl->setOutputString(res);
						curl->setInputString(input);
						curl->setPOST(true);
						curl->perform();
						if (curl->getResponseCode() != 200)
						{
							Bundle bres = Serialize::JsonSerializer().deserialize(res).as<Bundle>();
							throw ApiException(bres.get<std::string>("code"), bres.get<std::string>("message"), bres.get<std::string>("requestId"));
						}
						return res;
					}

					CVTagImageResult ComputerVision::tagImage(const std::string & url)
					{
						std::string json = Serialize::JsonSerializer().serialize(Bundle({ { "url", url } }));

						CVTagImageResult res;
						res.fromAnyValue(sendPost("/tag", json, false));
						return res;
					}

					CVTagImageResult ComputerVision::tagImage(const std::vector<uint8_t>& data)
					{
						std::string image(data.cbegin(), data.cend());
						CVTagImageResult res;
						res.fromAnyValue(sendPost("/tag", image, true));
						return res;
					}

					CVOCRResult ComputerVision::ocr(const std::string & url, const std::string& lang, bool detect_orientation)
					{
						std::string json = Serialize::JsonSerializer().serialize(Bundle({ { "url", url } }));

						CVOCRResult res;
						res.fromAnyValue(sendPost("/ocr?language=" + lang + "&detectOrientation=" + (detect_orientation?"true":"false"), json, false));
						return res;
					}

					CVOCRResult ComputerVision::ocr(const std::vector<uint8_t>& data, const std::string& lang, bool detect_orientation)
					{
						std::string image(data.cbegin(), data.cend());
						CVOCRResult res;
						res.fromAnyValue(sendPost("/ocr?language=" + lang + "&detectOrientation=" + (detect_orientation ? "true" : "false"), image, true));
						return res;
					}

					std::unique_ptr<Curl> ComputerVision::setupCurl(const std::string & url)
					{
						std::unique_ptr<Curl> curl = std::unique_ptr<Curl>(new Curl());
						curl->setURL("https://api.projectoxford.ai/vision/v1.0" + url);
						curl->setSSLCABundle("curl-ca-bundle.crt");
						curl->setHeaders({
							{ "Ocp-Apim-Subscription-Key", _ocp_api_key }
						});
						return curl;
					}

					Bundle ComputerVision::sendGet(const std::string & url)
					{
						auto curl = setupCurl(url);
						std::string res;
						curl->setOutputString(res);
						curl->perform();
						Bundle bres = Serialize::JsonSerializer().deserialize(res).as<Bundle>();
						if (curl->getResponseCode() != 200)
						{
							throw ApiException(bres.get<std::string>("code"), bres.get<std::string>("message"), bres.get<std::string>("requestId"));
						}
						return bres;
					}

					Bundle ComputerVision::sendPost(const std::string & url, const std::string& data, bool bin)
					{
						auto curl = setupCurl(url);
						curl->setHeaders({
							{ "Ocp-Apim-Subscription-Key", _ocp_api_key },
							{ "Content-Type", bin?"application/octet-stream":"application/json"}
						});
						std::string res;
						curl->setOutputString(res);
						curl->setInputString(data);
						curl->setPOST(true);
						curl->perform();
						Bundle bres = Serialize::JsonSerializer().deserialize(res).as<Bundle>();
						if (curl->getResponseCode() != 200)
						{
							throw ApiException(bres.get<std::string>("code"), bres.get<std::string>("message"), bres.get<std::string>("requestId"));
						}
						return bres;
					}
				}
			}
		}
	}
}