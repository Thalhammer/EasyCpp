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

		template<typename... Args>
		struct TypeInfoImpl;

		template<typename T, typename... Args>
		struct TypeInfoImpl<T, Args...>
		{
			static void getTypeInfo(std::vector<TypeInfo>& vect)
			{
				vect.push_back(TypeInfo::CreateInfo<T>());
				TypeInfoImpl<Args...>::getTypeInfo(vect);
			}
		};

		template<>
		struct TypeInfoImpl<>
		{
			static void getTypeInfo(std::vector<TypeInfo>& vect)
			{
			}
		};

		template<typename Result>
		static Result call_detail(std::function<Result()> fn, const AnyArray& a, size_t i)
		{
			return fn();
		}

		template<typename Result, typename Arg1>
		static Result call_detail(std::function<Result(Arg1)> fn, const AnyArray& a, size_t i)
		{
			if (a.size() <= i)
				throw std::out_of_range("Not enough parameters");
			return fn(a[i].as<Arg1>());
		}

		template<typename Result, typename Arg1, typename... Args>
		static Result call_detail(std::function<Result(Arg1, Args...)> fn, const AnyArray& a, size_t i)
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

		template<typename... Args>
		static std::vector<TypeInfo> getTypeInfo()
		{
			std::vector<TypeInfo> res;
			TypeInfoImpl<Args...>::getTypeInfo(res);
			return res;
		}

		template<typename Result, typename... Args>
		static AnyValue call(Result(*fn)(Args...), const AnyArray& vals)
		{
			auto nfn = [fn](Args... args) {
				return fn(args...);
			};
			return call_detail<Result, Args...>(nfn, vals, 0);
		}

		template<typename... Args>
		static AnyValue call(void(*fn)(Args...), const AnyArray& vals)
		{
			auto nfn = [fn](Args... args) {
				return fn(args...);
			};
			return call_detail<void, Args...>(nfn, vals, 0);
		}

		template<typename Result, typename... Args>
		static AnyValue call(std::function<Result(Args...)> fn, const AnyArray& vals)
		{
			return call_detail<Result, Args...>(fn, vals, 0);
		}

		template<typename... Args>
		static AnyValue call(std::function<void(Args...)> fn, const AnyArray& vals)
		{
			call_detail<void, Args...>(fn, vals, 0);
			return AnyValue();
		}
	};
}