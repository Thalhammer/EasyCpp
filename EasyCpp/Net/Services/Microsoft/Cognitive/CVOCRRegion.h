#pragma once
#include "CVOCRLine.h"
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
					class DLL_EXPORT CVOCRRegion : public Serialize::Serializable
					{
					public:
						CVOCRRegion();
						virtual ~CVOCRRegion();

						///<summary>Returns all lines as a single string by concatening with "\n"</summary>
						std::string getText() const;
						const CVBoundingBox& getBoundingBox() const;
						const std::vector<CVOCRLine>& getLines() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::vector<CVOCRLine> _lines;
						CVBoundingBox _bounding_box;
					};
				}
			}
		}
	}
}
