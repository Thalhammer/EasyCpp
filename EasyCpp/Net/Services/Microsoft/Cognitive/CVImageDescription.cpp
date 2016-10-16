#include "CVImageDescription.h"
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
					CVImageDescription::CVImageDescription()
					{
					}

					CVImageDescription::~CVImageDescription()
					{
					}

					const std::vector<std::string>& CVImageDescription::getTags() const
					{
						return _tags;
					}

					const std::vector<CVImageCaption>& CVImageDescription::getCaptions() const
					{
						return _captions;
					}

					AnyValue CVImageDescription::toAnyValue() const
					{
						return Bundle({
							{ "tags", toAnyArray(_tags)},
							{ "captions", toAnyArraySerialize(_captions)}
						});
					}

					void CVImageDescription::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_tags = fromAnyArray<std::string>(b.get<AnyArray>("tags"));
						_captions = fromAnyArray<CVImageCaption>(b.get<AnyArray>("captions"));
					}
				}
			}
		}
	}
}