#pragma once
#include "../../../../DllExport.h"
#include "../../../../Serialize/Serializable.h"
#include <vector>
#include <string>

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
					class DLL_EXPORT CVImageColor : public Serialize::Serializable
					{
					public:
						CVImageColor();
						virtual ~CVImageColor();

						const std::string& getDominantColorForeground() const;
						const std::string& getDominantColorBackground() const;
						const std::vector<std::string>& getDominantColors() const;
						const std::string& getAccentColor() const;
						bool isBWImage() const;


						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _dominantColorForeground;
						std::string _dominantColorBackground;
						std::vector<std::string> _dominantColors;
						std::string _accentColor;
						bool _isBWImg;
					};
				}
			}
		}
	}
}
