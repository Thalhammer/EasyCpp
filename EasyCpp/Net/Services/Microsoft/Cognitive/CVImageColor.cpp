#include "CVImageColor.h"
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
					CVImageColor::CVImageColor()
					{
					}

					CVImageColor::~CVImageColor()
					{
					}

					const std::string & CVImageColor::getDominantColorForeground() const
					{
						return _dominantColorForeground;
					}

					const std::string & CVImageColor::getDominantColorBackground() const
					{
						return _dominantColorBackground;
					}

					const std::vector<std::string>& CVImageColor::getDominantColors() const
					{
						return _dominantColors;
					}

					const std::string & CVImageColor::getAccentColor() const
					{
						return _accentColor;
					}

					bool CVImageColor::isBWImage() const
					{
						return _isBWImg;
					}

					AnyValue CVImageColor::toAnyValue() const
					{
						return Bundle({
							{ "dominantColorForeground", _dominantColorForeground },
							{ "dominantColorBackground", _dominantColorBackground },
							{ "dominantColors", toAnyArray(_dominantColors) },
							{ "accentColor", _accentColor },
							{ "isBWImg", _isBWImg }
						});
					}

					void CVImageColor::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_dominantColorForeground = b.get<std::string>("dominantColorForeground");
						_dominantColorBackground = b.get<std::string>("dominantColorBackground");
						_dominantColors = fromAnyArray<std::string>(b.get("dominantColors"));
						_accentColor = b.get<std::string>("accentColor");
						_isBWImg = b.get<bool>("isBWImg");
					}
				}
			}
		}
	}
}