#include "CVImageTag.h"
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
					CVImageTag::CVImageTag()
					{
					}

					CVImageTag::~CVImageTag()
					{
					}

					const std::string & CVImageTag::getName() const
					{
						return _name;
					}

					double CVImageTag::getConfidence() const
					{
						return _confidence;
					}

					AnyValue CVImageTag::toAnyValue() const
					{
						return Bundle({
							{ "name", _name },
							{ "confidence", _confidence }
						});
					}

					void CVImageTag::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_name = b.get<std::string>("name");
						_confidence = b.get<double>("confidence");
					}
				}
			}
		}
	}
}