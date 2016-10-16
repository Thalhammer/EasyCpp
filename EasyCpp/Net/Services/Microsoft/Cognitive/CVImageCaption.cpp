#include "CVImageCaption.h"
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
					CVImageCaption::CVImageCaption()
					{
					}

					CVImageCaption::~CVImageCaption()
					{
					}

					const std::string & CVImageCaption::getText() const
					{
						return _text;
					}

					double CVImageCaption::getConfidence() const
					{
						return _confidence;
					}

					AnyValue CVImageCaption::toAnyValue() const
					{
						return Bundle({
							{ "text", _text },
							{ "confidence", _confidence }
						});
					}

					void CVImageCaption::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_text = b.get<std::string>("text");
						_confidence = b.get<double>("confidence");
					}

				}
			}
		}
	}
}