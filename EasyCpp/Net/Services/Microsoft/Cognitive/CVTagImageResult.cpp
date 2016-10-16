#include "CVTagImageResult.h"
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
					CVTagImageResult::CVTagImageResult()
					{
					}

					CVTagImageResult::~CVTagImageResult()
					{
					}

					const std::vector<CVImageTag>& CVTagImageResult::getTags() const
					{
						return _tags;
					}

					const CVImageMetadata & CVTagImageResult::getMetadata() const
					{
						return _metadata;
					}

					const std::string & CVTagImageResult::getRequestId() const
					{
						return _request_id;
					}

					AnyValue CVTagImageResult::toAnyValue() const
					{
						return Bundle({
							{ "tags", toAnyArraySerialize(_tags) },
							{ "requestId", _request_id },
							{ "metadata", _metadata.toAnyValue() }
						});
					}

					void CVTagImageResult::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_tags = fromAnyArray<CVImageTag>(b.get("tags"));
						_request_id = b.get<std::string>("requestId");
						_metadata = b.get<CVImageMetadata>("metadata");
					}
				}
			}
		}
	}
}