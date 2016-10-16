#include "CVOCRLine.h"
#include "../../../../StringAlgorithm.h"
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
					CVOCRLine::CVOCRLine()
					{
					}

					CVOCRLine::~CVOCRLine()
					{
					}

					std::string CVOCRLine::getText() const
					{
						std::vector<std::string> words;
						for (auto& e : getWords()) {
							words.push_back(e.getText());
						}
						return implode<std::string>(" ", words);
					}

					const CVBoundingBox & CVOCRLine::getBoundingBox() const
					{
						return _bounding_box;
					}

					const std::vector<CVOCRWord>& CVOCRLine::getWords() const
					{
						return _words;
					}

					AnyValue CVOCRLine::toAnyValue() const
					{
						return Bundle({
							{ "boundingBox", _bounding_box.toAnyValue() },
							{ "words", toAnyArraySerialize(_words) }
						});
					}

					void CVOCRLine::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_bounding_box = b.get<CVBoundingBox>("boundingBox");
						_words = fromAnyArray<CVOCRWord>(b.get("words"));
					}
				}
			}
		}
	}
}