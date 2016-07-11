#pragma once
#include <vector>
#include "AnyValue.h"

//! \addtogroup Util
//! @{
namespace EasyCpp
{
	typedef std::vector<AnyValue> AnyArray;
	template<typename T>
	AnyArray toAnyArray(const std::vector<T>& arr)
	{
		AnyArray res;
		for (auto e : arr)
			res.push_back(e);
		return res;
	}

	template<typename T>
	AnyArray toAnyArraySerialize(const std::vector<T>& arr)
	{
		AnyArray res;
		for (auto e : arr)
			res.push_back(e.toAnyValue());
		return res;
	}

	template<typename T>
	std::vector<T> fromAnyArray(const AnyArray& arr)
	{
		std::vector<T> res;
		for (auto& e : arr)
			res.push_back(e.as<T>());
		return res;
	}
}
//! @}
