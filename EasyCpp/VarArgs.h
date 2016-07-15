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

		template<typename Result, typename Arg1>
		static Result call_detail(std::function<Result(Arg1)> fn, AnyArray& a, int i)
		{
			return fn(a[i].as<Arg1>());
		}

		template<typename Result, typename Arg1, typename... Args>
		static Result call_detail(std::function<Result(Arg1, Args...)> fn, AnyArray& a, int i)
		{
			auto nfn = [a, fn, i](Args... args) {
				return fn(a[i].as<Arg1>(), args...);
			};
			return call_detail<Result, Args...>(nfn, a, i + 1);
		}
	public:
		template<typename... Args>
		static AnyArray expandArgs(Args... args)
		{
			AnyArray res;
			expand(res, args...);
			return res;
		}

		template<typename Result, typename... Args>
		static AnyValue call(Result(*fn)(Args...), AnyArray& vals)
		{
			auto nfn = [fn](Args... args) {
				return fn(args...);
			};
			return call_detail<Result, Args...>(nfn, vals, 0);
		}

		template<typename Result, typename... Args>
		static AnyValue call(std::function<Result(Args...)> fn, AnyArray& vals)
		{
			return call_detail<Result, Args...>(fn, vals, 0);
		}
	};
}