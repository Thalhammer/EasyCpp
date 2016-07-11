#pragma once
#include "Serializable.h"
#include "../AnyValue.h"
#include <vector>

namespace EasyCpp
{
	namespace Serialize
	{
		template<typename T, typename _Alloc = typename std::allocator<T>>
		class Vector : public std::vector<T, _Alloc>, public Serializable
		{
		public:
			template<typename ...Params>
			Vector(Params&&... params)
				: std::vector<T, _Alloc>(std::forward<Params>(params)...)
			{}

			// Geerbt über Serializable
			virtual AnyValue toAnyValue() const override
			{
				std::vector<AnyValue> res;
				for (auto e : *this)
					res.push_back(e);
				return res;
			}

			virtual void fromAnyValue(const AnyValue & state) override
			{
				auto vect = state.as<std::vector<AnyValue>>();
				for (auto e : vect)
				{
					this->push_back(e.as<T>());
				}
			}
		};
	}
}