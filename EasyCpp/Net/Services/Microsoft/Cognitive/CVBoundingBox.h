#pragma once
#include "../../../../DllExport.h"
#include "../../../../Serialize/Serializable.h"
#include <string>

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
					class DLL_EXPORT CVBoundingBox : public Serialize::Serializable
					{
					public:
						CVBoundingBox();
						virtual ~CVBoundingBox();

						size_t getX() const;
						size_t getY() const;
						size_t getHeight() const;
						size_t getWidth() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						size_t _x;
						size_t _y;
						size_t _height;
						size_t _width;
					};
				}
			}
		}
	}
}
