#include "CVOCRResult.h"
#include "../../../../StringAlgorithm.h"
#include "../../../../Bundle.h"
#include "../../../../AnyArray.h"

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
					CVOCRResult::CVOCRResult()
					{
					}

					CVOCRResult::~CVOCRResult()
					{
					}

					std::string CVOCRResult::getText() const
					{
						std::vector<std::string> regions;
						for (auto& e : getRegions())
						{
							regions.push_back(e.getText());
						}
						return implode<std::string>("\n", regions);
					}

					const std::string & CVOCRResult::getLanguage() const
					{
						return _language;
					}

					double CVOCRResult::getTextAngle() const
					{
						return _text_angle;
					}

					CVOCRResult::Orientation CVOCRResult::getOrientation() const
					{
						return _orientation;
					}

					const std::vector<CVOCRRegion>& CVOCRResult::getRegions() const
					{
						return _regions;
					}

					AnyValue CVOCRResult::toAnyValue() const
					{
						std::string orientation = "";
						switch (_orientation) {
						case Orientation::Up: orientation = "Up"; break;
						case Orientation::Down: orientation = "Down"; break;
						case Orientation::Left: orientation = "Left"; break;
						case Orientation::Right: orientation = "Right"; break;
						default:
							throw std::runtime_error("Internal error");
						}
						return Bundle({
							{ "language", _language },
							{ "textAngle", _text_angle },
							{ "orientation", orientation },
							{ "regions", toAnyArraySerialize(_regions) }
						});
					}

					void CVOCRResult::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_language = b.get<std::string>("language");
						_text_angle = b.get<double>("textAngle");
						std::string str = stringToLower(b.get<std::string>("orientation"));
						_orientation = (str=="up"?Orientation::Up:(str=="down"?Orientation::Down:(str=="left"?Orientation::Left:(str=="right"?Orientation::Right:throw std::runtime_error("Invalid orientation")))));
						_regions = fromAnyArray<CVOCRRegion>(b.get("regions"));
					}
				}
			}
		}
	}
}