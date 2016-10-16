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
					class DLL_EXPORT CVImageFaceRectangle : public Serialize::Serializable
					{
					public:
						CVImageFaceRectangle();
						virtual ~CVImageFaceRectangle();

						size_t getTop() const;
						size_t getLeft() const;
						size_t getWidth() const;
						size_t getHeight() const;

						// Geerbt über Serializable
						virtual AnyValue toAnyValue() const override;
						virtual void fromAnyValue(const AnyValue & state) override;
					private:
						size_t _left;
						size_t _top;
						size_t _width;
						size_t _height;
					};
				}
			}
		}
	}
}
