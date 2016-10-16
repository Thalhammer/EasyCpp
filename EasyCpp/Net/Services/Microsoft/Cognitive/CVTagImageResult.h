#pragma once
#include "CVImageTag.h"
#include "CVImageMetadata.h"
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
					class DLL_EXPORT CVTagImageResult : public Serialize::Serializable
					{
					public:
						CVTagImageResult();
						virtual ~CVTagImageResult();

						const std::vector<CVImageTag>& getTags() const;
						const CVImageMetadata& getMetadata() const;
						const std::string& getRequestId() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _request_id;
						std::vector<CVImageTag> _tags;
						CVImageMetadata _metadata;
					};
				}
			}
		}
	}
}
