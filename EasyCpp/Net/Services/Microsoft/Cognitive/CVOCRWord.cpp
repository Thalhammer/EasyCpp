#include "CVOCRWord.h"
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
					CVOCRWord::CVOCRWord()
					{
					}

					CVOCRWord::~CVOCRWord()
					{
					}

					const std::string & CVOCRWord::getText() const
					{
						return _text;
					}

					const CVBoundingBox & CVOCRWord::getBoundingBox() const
					{
						return _bounding_box;
					}

					AnyValue CVOCRWord::toAnyValue() const
					{
						return Bundle({
							{ "text", _text },
							{ "boundingBox", _bounding_box.toAnyValue() }
						});
					}

					void CVOCRWord::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_text = b.get<std::string>("text");
						_bounding_box = b.get<CVBoundingBox>("boundingBox");
					}
				}
			}
		}
	}
}