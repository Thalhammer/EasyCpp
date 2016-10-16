#pragma once
#include "CVOCRRegion.h"
#include <vector>

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
					class DLL_EXPORT CVOCRResult : public Serialize::Serializable
					{
					public:
						enum class Orientation
						{
							Up,
							Down,
							Left,
							Right
						};

						CVOCRResult();
						virtual ~CVOCRResult();

						///<summary>Returns all regions as a single string by concatening with "\n"</summary>
						std::string getText() const;
						const std::string& getLanguage() const;
						double getTextAngle() const;
						Orientation getOrientation() const;
						const std::vector<CVOCRRegion>& getRegions() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::vector<CVOCRRegion> _regions;
						std::string _language;
						double _text_angle;
						Orientation _orientation;
					};
				}
			}
		}
	}
}
