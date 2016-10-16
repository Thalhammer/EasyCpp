#include "CVImageFaceRectangle.h"
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
					CVImageFaceRectangle::CVImageFaceRectangle()
					{
					}

					CVImageFaceRectangle::~CVImageFaceRectangle()
					{
					}

					size_t CVImageFaceRectangle::getTop() const
					{
						return _top;
					}

					size_t CVImageFaceRectangle::getLeft() const
					{
						return _left;
					}

					size_t CVImageFaceRectangle::getWidth() const
					{
						return _width;
					}

					size_t CVImageFaceRectangle::getHeight() const
					{
						return _height;
					}

					AnyValue CVImageFaceRectangle::toAnyValue() const
					{
						return Bundle({
							{ "left", _left },
							{ "top", _top },
							{ "width", _width },
							{ "height", _height }
						});
					}

					void CVImageFaceRectangle::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_left = b.get<size_t>("left");
						_top = b.get<size_t>("top");
						_width = b.get<size_t>("width");
						_height = b.get<size_t>("height");
					}
				}
			}
		}
	}
}