#include "CVDescribeImageResult.h"
#include "../../../../Bundle.h"

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
					CVDescribeImageResult::CVDescribeImageResult()
					{
					}

					CVDescribeImageResult::~CVDescribeImageResult()
					{
					}

					const CVImageDescription & CVDescribeImageResult::getDescription() const
					{
						return _description;
					}

					const CVImageMetadata & CVDescribeImageResult::getMetadata() const
					{
						return _metadata;
					}

					const std::string & CVDescribeImageResult::getRequestId() const
					{
						return _request_id;
					}

					AnyValue CVDescribeImageResult::toAnyValue() const
					{
						return Bundle({
							{ "description", _description.toAnyValue() },
							{ "requestId", _request_id },
							{ "metadata", _metadata.toAnyValue() }
						});
					}

					void CVDescribeImageResult::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_description = b.get<CVImageDescription>("description");
						_request_id = b.get<std::string>("requestId");
						_metadata = b.get<CVImageMetadata>("metadata");
					}
				}
			}
		}
	}
}