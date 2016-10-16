#include "CVOCRRegion.h"
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
					CVOCRRegion::CVOCRRegion()
					{
					}

					CVOCRRegion::~CVOCRRegion()
					{
					}

					std::string CVOCRRegion::getText() const
					{
						std::vector<std::string> lines;
						for (auto& e : getLines()) {
							lines.push_back(e.getText());
						}
						return implode<std::string>("\n", lines);
					}

					const CVBoundingBox & CVOCRRegion::getBoundingBox() const
					{
						return _bounding_box;
					}

					const std::vector<CVOCRLine>& CVOCRRegion::getLines() const
					{
						return _lines;
					}

					AnyValue CVOCRRegion::toAnyValue() const
					{
						return Bundle({
							{ "boundingBox", _bounding_box.toAnyValue() },
							{ "lines", toAnyArraySerialize(_lines) }
						});
					}

					void CVOCRRegion::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_bounding_box = b.get<CVBoundingBox>("boundingBox");
						_lines = fromAnyArray<CVOCRLine>(b.get("lines"));
					}
				}
			}
		}
	}
}