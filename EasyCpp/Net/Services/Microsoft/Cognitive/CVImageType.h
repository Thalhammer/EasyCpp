#pragma once
#include "../../../../DllExport.h"
#include "../../../../Serialize/Serializable.h"

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
					class DLL_EXPORT CVImageType : public Serialize::Serializable
					{
					public:
						enum class ClipartType
						{
							NonClipart = 0,
							Ambiguous,
							NormalClipart,
							GoodClipart
						};
						enum class LineDrawingType
						{
							NonLineDrawing = 0,
							LineDrawing
						};
						CVImageType();
						virtual ~CVImageType();

						ClipartType getClipartType() const;
						LineDrawingType getLineDrawingType() const;

						bool isClipart() const;
						bool isLineDrawing() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						ClipartType _clipart_type;
						LineDrawingType _line_drawing_type;
					};
				}
			}
		}
	}
}
