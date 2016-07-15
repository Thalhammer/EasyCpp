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
		private:
			std::function<Result(Args...)> _function;
		};

	public:
		///<summary>Construct a empty function.</summary>
		AnyFunction();
		~AnyFunction();

		template<typename Result, typename... Args>
		AnyFunction(std::function<Result(Args...)> fn)
		{
			_fn = std::make_shared<Function<Result, Args...>>(fn);
		}

		///<summary>Try to call function using supplied arguments.</summary>
		AnyValue call(const AnyArray& args);
		///<summary>Get parameter information</summary>
		std::vector<TypeInfo> getParameterTypes();
		///<summary>Get return information</summary>
		TypeInfo getReturnType();
	private:
		std::shared_ptr<FunctionBase> _fn;
	};
}