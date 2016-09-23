#include "Category.h"
#include "../../../Bundle.h"
#include "../../../AnyArray.h"

namespace EasyCpp
{
	namespace Net
	{
		namespace Services
		{
			namespace Spotify
			{
				Category::Category()
				{
				}

				Category::~Category()
				{
				}

				const std::string & Category::getHref() const
				{
					return _href;
				}

				const std::string & Category::getID() const
				{
					return _id;
				}

				const std::string & Category::getName() const
				{
					return _name;
				}

				const std::vector<Image>& Category::getIcons() const
				{
					return _icons;
				}

				AnyValue Category::toAnyValue() const
				{
					return Bundle({
						{ "href", _href },
						{ "icons", toAnyArraySerialize(_icons) },
						{ "id", _id },
						{ "name", _name }
					});
				}

				void Category::fromAnyValue(const AnyValue & state)
				{
					Bundle b = state.as<Bundle>();
					_href = b.get<std::string>("href");
					_icons = fromAnyArray<Image>(b.get<AnyArray>("icons"));
					_id = b.get<std::string>("id");
					_name = b.get<std::string>("name");
				}
			}
		}
	}
}