#include "CVImageMetadata.h"
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
					CVImageMetadata::CVImageMetadata()
					{
					}

					CVImageMetadata::~CVImageMetadata()
					{
					}

					const std::string & CVImageMetadata::getFormat() const
					{
						return _format;
					}

					size_t CVImageMetadata::getWidth() const
					{
						return _width;
					}

					size_t CVImageMetadata::getHeight() const
					{
						return _height;
					}

					AnyValue CVImageMetadata::toAnyValue() const
					{
						return Bundle({
							{ "format", _format },
							{ "width", _width },
							{ "height", _height }
						});
					}

					void CVImageMetadata::fromAnyValue(const AnyValue & state)
					{
						Bundle b = state.as<Bundle>();
						_format = b.get<std::string>("format");
						_width = b.get<size_t>("width");
						_height = b.get<size_t>("height");
					}
				}
			}
		}
	}
}