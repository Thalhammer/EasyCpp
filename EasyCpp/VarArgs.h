#pragma once
#include "AnyArray.h"
#include <vector>

namespace EasyCpp
{
	class VarArgs
	{
	private:
		VarArgs() = delete;
		~VarArgs() = delete;

		template<typename T>
		static void expand(AnyArray& vect, T arg)
		{
			vect.push_back(arg);
		}

		template<typename T, typename... Args>
		static void expand(AnyArray& vect, T arg, Args... args)
		{
			vect.push_back(arg);
			expand(vect, args...);
		}
	public:
		template<typename... Args>
		static AnyArray expandArgs(Args... args)
		{
			AnyArray res;
			expand(res, args...);
			return res;
		}
	};
}