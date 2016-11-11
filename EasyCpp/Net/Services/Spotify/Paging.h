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
				class DLL_EXPORT Paging : public Serialize::Serializable
				{
				public:
					Paging() : _item_key("items") {}
					Paging(const std::string& item_key) : _item_key(item_key) {}
					virtual ~Paging() {}

					const std::string& getHref() const { return _href; }
					const std::vector<T>& getItems() const { return _items; }
					int getLimit() const { return _limit; }
					const std::string& getNext() const { return _next; }
					int getOffset() const { return _offset; }
					const std::string& getPrevious() const { return _previous; }
					int getTotal() const { return _total; }
					const std::string& getAfter() const { return _after; }

					// Geerbt über Serializable
					virtual AnyValue toAnyValue() const override
					{
						std::vector<AnyValue> items;
						for (auto e : _items)
							items.push_back(e.toAnyValue());
						Bundle res({
							{ "href", _href },
							{ _item_key, _items },
							{ "limit", _limit },
							{ "next", _next },
							{ "total", _total }
						});
						if (_after == "") {
							res.set("offset", _offset);
							res.set("previous", _previous);
						} else {
							res.set("cursors", Bundle({
								{"after", _after}
							}));
						}
						return res;
					}

					virtual void fromAnyValue(const AnyValue & state) override
					{
						Bundle b = state.as<Bundle>();
						_href = b.get<std::string>("href");
						_items = fromAnyArray<T>(b.get<AnyArray>(_item_key));
						_limit = b.get<int>("limit");
						_next = b.get("next").isType<nullptr_t>() ? "" : b.get<std::string>("next");
						if (b.isSet("cursors")) {
							_after = b.get<Bundle>("cursors").get<std::string>("after");
						}
						else {
							_offset = b.get<int>("offset");
							_previous = b.get("previous").isType<nullptr_t>() ? "" : b.get<std::string>("previous");
						}
						_total = b.get<int>("total");
					}
				private:
					std::string _item_key;

					std::string _href;
					std::vector<T> _items;
					int _limit;
					std::string _next;
					int _offset;
					std::string _previous;
					int _total;

					std::string _after;
				};
			}
		}
	}
}
