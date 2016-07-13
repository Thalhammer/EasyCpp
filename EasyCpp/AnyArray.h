#pragma once
#include <vector>
#include "AnyValue.h"

namespace EasyCpp
{
	/// <summary>A vector of AnyValues.</summary>
	typedef std::vector<AnyValue> AnyArray;
	template<typename T>
	/// <summary>Convert a vector of T to a AnyArray.</summary>
	AnyArray toAnyArray(const std::vector<T>& arr)
	{
		AnyArray res;
		for (auto e : arr)
			res.push_back(e);
		return res;
	}

	template<typename T>
	/// <summary>Convert a vector of T to a AnyArray by serializing the vectors elements.</summary>
	AnyArray toAnyArraySerialize(const std::vector<T>& arr)
	{
		AnyArray res;
		for (auto e : arr)
			res.push_back(e.toAnyValue());
		return res;
	}

	template<typename T>
	/// <summary>Convert a vector of AnyValues to a vector of type T</summary>
	std::vector<T> fromAnyArray(const AnyArray& arr)
	{
		std::vector<T> res;
		for (auto& e : arr)
			res.push_back(e.as<T>());
		return res;
	}
}
