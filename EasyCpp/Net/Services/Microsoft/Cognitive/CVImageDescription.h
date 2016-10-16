#pragma once
#include "CVImageCaption.h"
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
					class DLL_EXPORT CVImageDescription : public Serialize::Serializable
					{
					public:
						CVImageDescription();
						virtual ~CVImageDescription();

						const std::vector<std::string>& getTags() const;
						const std::vector<CVImageCaption>& getCaptions() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::vector<std::string> _tags;
						std::vector<CVImageCaption> _captions;
					};
				}
			}
		}
	}
}
