#pragma once
#include "AnyArray.h"
#include <vector>

namespace EasyCpp
{
	template<typename... Args>
	struct VarArgsTypeInfoImpl;

	template<typename T, typename... Args>
	struct VarArgsTypeInfoImpl<T, Args...>
	{
		static void getTypeInfo(std::vector<TypeInfo>& vect)
		{
			vect.push_back(TypeInfo::CreateInfo<T>());
			VarArgsTypeInfoImpl<Args...>::getTypeInfo(vect);
		}
	};

	template<>
	struct VarArgsTypeInfoImpl<>
	{
		static void getTypeInfo(std::vector<TypeInfo>& vect)
		{
		}
	};

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

		template<typename Result>
		static Result call_detail(std::function<Result()> fn, const AnyArray& a, size_t i)
		{
			if (a.size() < i)
				throw std::out_of_range("Not enough parameters");
			return fn();
		}

		template<typename Result, typename Arg1>
		static Result call_detail(std::function<Result(Arg1)> fn, const AnyArray& a, size_t i)
		{
			auto nfn = [a, fn, i]() {
				auto arg1 = a[i].as<Arg1>();
				return fn(arg1);
			};
			return call_detail<Result>(nfn, a, i + 1);
		}

		template<typename Result, typename Arg1, typename Arg2, typename... Args>
		static Result call_detail(std::function<Result(Arg1, Arg2, Args...)> fn, const AnyArray& a, size_t i)
		{
			auto nfn = [a, fn, i](Args... args) {
				auto arg1 = a[i].as<Arg1>();
				auto arg2 = a[i + 1].as<Arg2>();
				return fn(arg1, arg2, args...);
			};
			return call_detail<Result, Args...>(nfn, a, i + 2);
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
			VarArgsTypeInfoImpl<Args...>::getTypeInfo(res);
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
