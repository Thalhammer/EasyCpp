#pragma once
#include "AnyArray.h"
#include "VarArgs.h"

namespace EasyCpp
{
	class DLL_EXPORT AnyFunction
	{
	public:
		class FunctionBase
		{
		public:
			FunctionBase() {}
			virtual ~FunctionBase() {}
			virtual AnyValue call(const AnyArray& args) = 0;
			virtual std::vector<TypeInfo> getParameterTypes() = 0;
			virtual TypeInfo getReturnType() = 0;
			virtual bool hasVarArgs() = 0;
			virtual bool hasVarReturnValue() = 0;
		};

		template<typename Result, typename... Args>
		class Function : public FunctionBase
		{
		public:
			Function(std::function<Result(Args...)> fn) { _function = fn; }
			virtual ~Function() {}
			virtual AnyValue call(const AnyArray & args) override
			{
				return VarArgs::call(_function, args);
			}
			virtual std::vector<TypeInfo> getParameterTypes() override
			{
				return VarArgs::getTypeInfo<Args...>();
			}
			virtual TypeInfo getReturnType() override
			{
				return TypeInfo::CreateInfo<Result>();
			}
			virtual bool hasVarArgs() override
			{
				return false;
			}
			virtual bool hasVarReturnValue() override
			{
				return false;
			}
		private:
			std::function<Result(Args...)> _function;
		};

		class DynamicFunction : public FunctionBase
		{
		public:
			DynamicFunction(std::function<AnyValue(const AnyArray&)> fn) { _function = fn; }
			virtual ~DynamicFunction() {}
			virtual AnyValue call(const AnyArray & args) override
			{
				return _function(args);
			}
			virtual std::vector<TypeInfo> getParameterTypes() override
			{
				return{};
			}
			virtual TypeInfo getReturnType() override
			{
				return TypeInfo::CreateInfo<nullptr_t>();
			}
			virtual bool hasVarArgs() override
			{
				return true;
			}
			virtual bool hasVarReturnValue() override
			{
				return true;
			}
		private:
			std::function<AnyValue(const AnyArray&)> _function;
		};

		AnyFunction(std::shared_ptr<FunctionBase> fn)
			: _fn(fn)
		{}
	public:
		///<summary>Construct a empty function.</summary>
		AnyFunction();
		~AnyFunction();

		template<typename Result, typename... Args>
		static AnyFunction fromFunction(std::function<Result(Args...)> fn)
		{
			return AnyFunction(std::make_shared<Function<Result, Args...>>(fn));
		}

		template<typename Obj, typename Result, typename ...Args>
		static AnyFunction fromFunction(Result(Obj::*fn)(Args...))
		{
			return AnyFunction(std::make_shared<Function<Result, Obj*, Args...>>([fn](Obj* obj, Args&&... args) {
				return (*obj.*fn)(std::forward<Args>(args)...);
			}));
		}

		template<typename Result, typename ...Args>
		static AnyFunction fromFunction(Result(*fn)(Args...))
		{
			return AnyFunction(std::make_shared<Function<Result, Args...>>([fn](Args&&... args) {
				return (*fn)(std::forward<Args>(args)...);
			}));
		}

		static AnyFunction fromDynamicFunction(std::function<AnyValue(const AnyArray& args)> fn)
		{
			return AnyFunction(std::make_shared<DynamicFunction>(fn));
		}

		///<summary>Try to call function using supplied arguments.</summary>
		AnyValue call(const AnyArray& args);
		///<summary>Get parameter information</summary>
		std::vector<TypeInfo> getParameterTypes();
		///<summary>Get return information</summary>
		TypeInfo getReturnType();
		///<summary>Check if this functions accepts variable arguments</summary>
		bool hasVarArgs();
		///<summary>Check if this function returns variable result types</summary>
		bool hasVarReturnValue();
	private:
		std::shared_ptr<FunctionBase> _fn;
	};
}