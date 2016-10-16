#pragma once
#include "CVImageDescription.h"
#include "CVImageMetadata.h"

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
					class DLL_EXPORT CVDescribeImageResult : public Serialize::Serializable
					{
					public:
						CVDescribeImageResult();
						virtual ~CVDescribeImageResult();

						const CVImageDescription& getDescription() const;
						const CVImageMetadata& getMetadata() const;
						const std::string& getRequestId() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						std::string _request_id;
						CVImageDescription _description;
						CVImageMetadata _metadata;
					};
				}
			}
		}
	}
}
