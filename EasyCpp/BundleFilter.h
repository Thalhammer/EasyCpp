#pragma once
#include <string>
#include <vector>
#include "Bundle.h"

namespace EasyCpp
{
	/**
	* Allows to filter a bundle's content based on the a selector.
	* For example the following selector would only return the value "id" and the value "name" of it's subbundle "album".
	* {id,album{id}}
	*/
	class DLL_EXPORT BundleFilter
	{
	public:
		BundleFilter(std::string filter);
		virtual ~BundleFilter();

		Bundle filterBundle(const Bundle& in);
	private:
		std::string _str_filter;
		static Bundle filter(const Bundle& in, const Bundle& root, const std::string& filter);
		static std::vector<std::string> safeExplode(const std::string& filter);
	};
}
