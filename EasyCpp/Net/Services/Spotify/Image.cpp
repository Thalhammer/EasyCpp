#include "Image.h"
#include "../../../Bundle.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Image::~Image()
				{
				}

				const std::string& Image::getURL() const
				{
					return _url;
				}

				uint64_t Image::getHeight() const
				{
					return _height;
				}

				uint64_t Image::getWidth() const
				{
					return _width;
				}

				AnyValue Image::toAnyValue() const
				{
					return Bundle({
						{ "height", _height },
						{ "width", _width },
						{ "url", _url }
					});
				}

				void Image::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_height = b.get<uint64_t>("height");
					_width = b.get<uint64_t>("width");
					_url = b.get<std::string>("url");
				}

				Image::Image()
				{
				}
			}
		}
	}
}
