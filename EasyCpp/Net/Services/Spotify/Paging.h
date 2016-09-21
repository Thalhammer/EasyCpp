#pragma once
#include "../../../Serialize/Serializable.h"
#include "../../../DllExport.h"
#include <string>
#include <vector>
#include <cstddef>
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
				template<typename T>
				class Paging : public Serialize::Serializable
				{
				public:
					Paging() {}
					virtual ~Paging() {}

					const std::string& getHref() const { return _href; }
					const std::vector<T>& getItems() const { return _items; }
					int getLimit() const { return _limit; }
					const std::string& getNext() const { return _next; }
					int getOffset() const { return _offset; }
					const std::string& getPrevious() const { return _previous; }
					int getTotal() const { return _total; }

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override
					{
						std::vector<AnyValue> items;
						for (auto e : _items)
							items.push_back(e.toAnyValue());
						return Bundle({
							{ "href", _href },
							{ "items", _items },
							{ "limit", _limit },
							{ "next", _next },
							{ "offset", _offset },
							{ "previous", _previous },
							{ "total", _total }
						});
					}

					virtual void fromAnyValue(const AnyValue & state) override
					{
						Bundle b = state.as<Bundle>();
						_href = b.get<std::string>("href");
						_items = fromAnyArray<T>(b.get<AnyArray>("items"));
						_limit = b.get<int>("limit");
						_next = b.get("next").isType<nullptr_t>() ? "" : b.get<std::string>("next");
						_offset = b.get<int>("offset");
						_previous = b.get("previous").isType<nullptr_t>() ? "" : b.get<std::string>("previous");
						_total = b.get<int>("total");
					}
				private:
					std::string _href;
					std::vector<T> _items;
					int _limit;
					std::string _next;
					int _offset;
					std::string _previous;
					int _total;
				};
			}
		}
	}
}
