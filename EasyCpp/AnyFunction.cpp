#include "AnyFunction.h"

namespace EasyCpp
{
	AnyFunction::AnyFunction()
	{
	}

	AnyFunction::~AnyFunction()
	{
	}

	AnyValue AnyFunction::call(const AnyArray & args)
	{
		if (!_fn)
			throw std::runtime_error("Function is empty");
		return _fn->call(args);
	}

	std::vector<TypeInfo> AnyFunction::getParameterTypes()
	{
		if (!_fn)
			throw std::runtime_error("Function is empty");
		return _fn->getParameterTypes();
	}

	TypeInfo AnyFunction::getReturnType()
	{
		if (!_fn)
			throw std::runtime_error("Function is empty");
		return _fn->getReturnType();
	}
}