#include "BundleFilter.h"
#include "RuntimeException.h"
#include "StringAlgorithm.h"

namespace EasyCpp
{
	BundleFilter::BundleFilter(std::string filter)
		: _str_filter(filter)
	{
	}

	BundleFilter::~BundleFilter()
	{
	}

	Bundle BundleFilter::filterBundle(const Bundle & in)
	{
		return filter(in, in, _str_filter);
	}

	Bundle BundleFilter::filter(const Bundle & in, const Bundle & root, const std::string & filter)
	{
		Bundle res;
		if (filter[0] != '{' || filter[filter.size() - 1] != '}')
			throw RuntimeException("Filter needs to start with '{' and end with '}'.");

		std::string p_filter = filter.substr(1, filter.size() - 2);
		//Explode safely
		std::vector<std::string> items = safeExplode(p_filter);

		for (auto& item : items)
		{
			auto pos = item.find('{');
			if (pos != std::string::npos)
			{
				// Bundle subselect
				std::string name = item.substr(0, pos);
				std::string subfilter = item.substr(pos);
				Bundle b = in.get<Bundle>(name);
				b = BundleFilter::filter(b, root, subfilter);
				res.set(name, b);
			}
			else
			{
				res.set(item, in.get(item));
			}
		}

		return res;
	}

	std::vector<std::string> BundleFilter::safeExplode(const std::string & filter)
	{
		std::vector<std::string> items;
		int depth = 0;
		std::string t_str;
		for (size_t i = 0; i< filter.size(); i++)
		{
			const auto& c = filter[i];
			if (c == ',' && depth == 0)
			{
				items.push_back(trim(t_str));
				t_str = "";
			}
			else if (c == '{')
			{
				t_str += c;
				depth++;
			}
			else if (c == '}')
			{
				t_str += c;
				depth--;
				if (depth < 0)
					throw RuntimeException("Invalid filter, unexpected '}' on pos " + std::to_string(i));
			}
			else
			{
				t_str += c;
			}
		}
		if (depth != 0)
			throw RuntimeException("Invalid filter, missing '}'");
		items.push_back(trim(t_str));
		return items;
	}

}