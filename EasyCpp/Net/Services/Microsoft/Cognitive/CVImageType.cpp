#include "CVImageType.h"
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
					CVImageType::CVImageType()
					{
					}

					CVImageType::~CVImageType()
					{
					}

					CVImageType::ClipartType CVImageType::getClipartType() const
					{
						return _clipart_type;
					}

					CVImageType::LineDrawingType CVImageType::getLineDrawingType() const
					{
						return _line_drawing_type;
					}

					bool CVImageType::isClipart() const
					{
						return getClipartType() != ClipartType::NonClipart;
					}

					bool CVImageType::isLineDrawing() const
					{
						return getLineDrawingType() != LineDrawingType::NonLineDrawing;
					}

					AnyValue CVImageType::toAnyValue() const
					{
						return Bundle({
							{ "clipArtType", (size_t)_clipart_type },
							{ "lineDrawingType", (size_t)_line_drawing_type }
						});
					}

					void CVImageType::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_clipart_type = (ClipartType)b.get<size_t>("clipArtType");
						_line_drawing_type = (LineDrawingType)b.get<size_t>("lineDrawingType");
					}
				}
			}
		}
	}
}