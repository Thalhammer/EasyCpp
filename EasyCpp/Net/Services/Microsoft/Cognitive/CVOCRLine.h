#pragma once
#include "CVOCRWord.h"
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
					class DLL_EXPORT CVOCRLine : public Serialize::Serializable
					{
					public:
						CVOCRLine();
						virtual ~CVOCRLine();

						///<summary>Returns all words as a single string by concatening with " "</summary>
						std::string getText() const;
						const CVBoundingBox& getBoundingBox() const;
						const std::vector<CVOCRWord>& getWords() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::vector<CVOCRWord> _words;
						CVBoundingBox _bounding_box;
					};
				}
			}
		}
	}
}
