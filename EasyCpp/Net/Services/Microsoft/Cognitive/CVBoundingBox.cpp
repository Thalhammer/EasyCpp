#include "CVBoundingBox.h"
#include "../../../../AnyValue.h"
#include "../../../../StringAlgorithm.h"

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
					CVBoundingBox::CVBoundingBox()
					{
					}

					CVBoundingBox::~CVBoundingBox()
					{
					}

					size_t CVBoundingBox::getX() const
					{
						return _x;
					}

					size_t CVBoundingBox::getY() const
					{
						return _y;
					}

					size_t CVBoundingBox::getHeight() const
					{
						return _height;
					}

					size_t CVBoundingBox::getWidth() const
					{
						return _width;
					}

					AnyValue CVBoundingBox::toAnyValue() const
					{
						std::string res;
						res += std::to_string(_x) + ",";
						res += std::to_string(_y) + ",";
						res += std::to_string(_width) + ",";
						res += std::to_string(_height);
						return res;
					}

					void CVBoundingBox::fromAnyValue(const AnyValue & state)
					{
						auto parts = stringSplit<std::string>(",", state.as<std::string>());
						if (parts.size() != 4) throw std::runtime_error("Invalid AnyValue");
						_x = std::stoul(parts[0]);
						_y = std::stoul(parts[1]);
						_width = std::stoul(parts[2]);
						_height = std::stoul(parts[3]);
					}
				}
			}
		}
	}
}