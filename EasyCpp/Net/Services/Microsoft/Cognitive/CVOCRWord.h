#pragma once
#include "CVBoundingBox.h"
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
					class DLL_EXPORT CVOCRWord : public Serialize::Serializable
					{
					public:
						CVOCRWord();
						virtual ~CVOCRWord();

						const std::string& getText() const;
						const CVBoundingBox& getBoundingBox() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _text;
						CVBoundingBox _bounding_box;
					};
				}
			}
		}
	}
}
